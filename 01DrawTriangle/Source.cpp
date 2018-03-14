#define OEMRESOURCE
#include <vector>
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace ATL;
using namespace DirectX;

const wchar_t* NAME = L"DirectX11";
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

LRESULT CALLBACK ProceedMessage(
    HWND window,
    UINT message,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (message == WM_DESTROY)
        PostQuitMessage(0);

    return DefWindowProcW(window, message, wParam, lParam);
}

void CompileShader(
    const wchar_t* const filePath,
    const char* const entryPoint,
    const char* const shaderModel,
    ID3DBlob** out
)
{
    CComPtr<ID3DBlob> errorBlob = nullptr;
    D3DCompileFromFile(
        filePath,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        shaderModel,
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        out,
        &errorBlob
    );

    if (errorBlob != nullptr)
    {
        OutputDebugStringA(
            static_cast<char*>(errorBlob->GetBufferPointer())
        );
    }
}

struct Vertex
{
    XMFLOAT3 position;
    XMFLOAT3 color;
};

struct Constant
{
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR, int)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    HCURSOR cursor = static_cast<HCURSOR>(LoadImageW(
        nullptr,
        MAKEINTRESOURCEW(OCR_NORMAL),
        IMAGE_CURSOR,
        0,
        0,
        LR_SHARED
    ));

    WNDCLASSW windowClass = {};
    windowClass.lpfnWndProc = ProceedMessage;
    windowClass.hInstance = instance;
    windowClass.hCursor = cursor;
    windowClass.lpszClassName = NAME;

    RegisterClassW(&windowClass);

    HWND window = CreateWindowW(
        NAME,
        NAME,
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        0,
        0,
        nullptr,
        nullptr,
        instance,
        nullptr
    );

    RECT windowRect = {};
    RECT clientRect = {};

    GetWindowRect(window, &windowRect);
    GetClientRect(window, &clientRect);

    int w = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + WIDTH;
    int h = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + HEIGHT;
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    SetWindowPos(
        window,
        nullptr,
        x,
        y,
        w,
        h,
        SWP_FRAMECHANGED
    );

    ShowWindow(window, SW_SHOWNORMAL);

    vector<D3D_FEATURE_LEVEL> featureLevels
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = WIDTH;
    swapChainDesc.BufferDesc.Height = HEIGHT;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = window;
    swapChainDesc.Windowed = true;

    CComPtr<ID3D11Device> device = nullptr;
    CComPtr<IDXGISwapChain> swapChain = nullptr;
    CComPtr<ID3D11DeviceContext> context = nullptr;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        featureLevels.data(),
        featureLevels.size(),
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &swapChain,
        &device,
        nullptr,
        &context
    );

    context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    CComPtr<ID3D11Texture2D> renderTexture = nullptr;

    swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&renderTexture)
    );

    CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;

    device->CreateRenderTargetView(
        renderTexture,
        nullptr,
        &renderTargetView
    );

    D3D11_VIEWPORT viewPort = {};
    viewPort.Width = WIDTH;
    viewPort.Height = HEIGHT;
    viewPort.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewPort);

    vector<Vertex> vertices
    {
        { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
    };

    CComPtr<ID3D11Buffer> vertexBuffer = nullptr;

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
    vertexSubresourceData.pSysMem = vertices.data();

    device->CreateBuffer(
        &vertexBufferDesc,
        &vertexSubresourceData,
        &vertexBuffer
    );

    CComPtr<ID3D11VertexShader> vertexShader = nullptr;
    CComPtr<ID3DBlob> vertexShaderBlob = nullptr;

    CompileShader(L"Shader.hlsl", "VS", "vs_5_0", &vertexShaderBlob);

    device->CreateVertexShader(
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr,
        &vertexShader
    );

    CComPtr<ID3D11PixelShader> pixelShader = nullptr;
    CComPtr<ID3DBlob> pixelShaderBlob = nullptr;

    CompileShader(L"Shader.hlsl", "PS", "ps_5_0", &pixelShaderBlob);

    device->CreatePixelShader(
        pixelShaderBlob->GetBufferPointer(),
        pixelShaderBlob->GetBufferSize(),
        nullptr,
        &pixelShader
    );

    CComPtr<ID3D11InputLayout> inputLayout = nullptr;

    vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    device->CreateInputLayout(
        inputElementDesc.data(),
        inputElementDesc.size(),
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        &inputLayout
    );

    CComPtr<ID3D11Buffer> constantBuffer = nullptr;

    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = sizeof(Constant);
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);

    Constant constant;
    constant.view = XMMatrixTranspose(
        XMMatrixTranslation(0.0f, 0.0f, 5.0f)
    );

    constant.projection = XMMatrixTranspose(
        XMMatrixPerspectiveFovLH(
            XMConvertToRadians(60.0f),
            (float)WIDTH / HEIGHT,
            0.1f,
            100.0f
        )
    );

    float angle = 0.0f;

    while (true)
    {
        MSG message = {};

        if (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
                break;

            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
        else
        {
            angle += 0.01f;
            constant.world = XMMatrixTranspose(
                XMMatrixRotationY(angle)
            );

            context->OMSetRenderTargets(1, &renderTargetView.p, nullptr);

            context->UpdateSubresource(constantBuffer, 0, nullptr, &constant, 0, 0);
            context->VSSetConstantBuffers(0, 1, &constantBuffer.p);

            UINT stride = sizeof(Vertex);
            UINT offset = 0;
            context->IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);

            context->VSSetShader(vertexShader, nullptr, 0);
            context->PSSetShader(pixelShader, nullptr, 0);
            context->IASetInputLayout(inputLayout);

            static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            context->ClearRenderTargetView(renderTargetView, color);

            context->Draw(vertices.size(), 0);

            swapChain->Present(1, 0);
        }
    }

    return 0;
}
