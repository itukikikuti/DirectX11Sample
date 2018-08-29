#define OEMRESOURCE
#include <string>
#include <vector>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using Microsoft::WRL::ComPtr;

LRESULT CALLBACK ProceedMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY)
        PostQuitMessage(0);

    return DefWindowProcW(window, message, wParam, lParam);
}

void CompileShader(const std::string& source, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out)
{
    ComPtr<ID3DBlob> error = nullptr;
    D3DCompile(source.c_str(), source.length(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0, out, error.GetAddressOf());

    if (error != nullptr)
    {
        OutputDebugStringA((char*)error->GetBufferPointer());
        MessageBoxA(nullptr, (char*)error->GetBufferPointer(), "Shader error", MB_ICONERROR | MB_OK);
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

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    const wchar_t* className = L"DirectX 11";
    const int width = 640;
    const int height = 480;

    HINSTANCE instance = GetModuleHandleW(nullptr);

    WNDCLASSW windowClass = {};
    windowClass.lpfnWndProc = ProceedMessage;
    windowClass.hInstance = instance;
    windowClass.hCursor = (HCURSOR)LoadImageW(nullptr, MAKEINTRESOURCEW(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED);
    windowClass.lpszClassName = className;
    RegisterClassW(&windowClass);

    HWND window = CreateWindowW(className, className, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, nullptr, nullptr, instance, nullptr);

    RECT windowRect = {};
    RECT clientRect = {};
    GetWindowRect(window, &windowRect);
    GetClientRect(window, &clientRect);

    int w = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + width;
    int h = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + height;
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    SetWindowPos(window, nullptr, x, y, w, h, SWP_FRAMECHANGED);

    ShowWindow(window, SW_SHOWNORMAL);

    std::vector<D3D_DRIVER_TYPE> driverTypes
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
        D3D_DRIVER_TYPE_SOFTWARE,
    };

    std::vector<D3D_FEATURE_LEVEL> featureLevels
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = window;
    swapChainDesc.Windowed = true;

    ComPtr<ID3D11Device> device = nullptr;
    ComPtr<IDXGISwapChain> swapChain = nullptr;
    ComPtr<ID3D11DeviceContext> context = nullptr;

    for (size_t i = 0; i < driverTypes.size(); i++)
    {
        HRESULT r = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i], nullptr, 0, featureLevels.data(), (UINT)featureLevels.size(), D3D11_SDK_VERSION, &swapChainDesc, swapChain.GetAddressOf(), device.GetAddressOf(), nullptr, context.GetAddressOf());

        if (SUCCEEDED(r))
            break;
    }

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ComPtr<ID3D11Texture2D> renderTexture = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(renderTexture.GetAddressOf()));

    ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
    device->CreateRenderTargetView(renderTexture.Get(), nullptr, renderTargetView.GetAddressOf());

    D3D11_VIEWPORT viewPort = {};
    viewPort.Width = width;
    viewPort.Height = height;
    viewPort.MaxDepth = 1.0f;
    context->RSSetViewports(1, &viewPort);

    std::vector<Vertex> vertices
    {
        { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
        { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * (UINT)vertices.size();
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
    vertexSubresourceData.pSysMem = vertices.data();

    ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
    device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());

    const std::string shader(
        "cbuffer Constant : register(b0)"
        "{"
        "    matrix world;"
        "    matrix view;"
        "    matrix projection;"
        "};"
        "struct Vertex"
        "{"
        "    float4 position : POSITION;"
        "    float3 color : COLOR;"
        "};"
        "struct Pixel"
        "{"
        "    float4 position : SV_POSITION;"
        "    float3 color : COLOR;"
        "};"
        "Pixel VS(Vertex vertex)"
        "{"
        "    Pixel output;"
        "    output.position = mul(vertex.position, world);"
        "    output.position = mul(output.position, view);"
        "    output.position = mul(output.position, projection);"
        "    output.color = vertex.color;"
        "    return output;"
        "}"
        "float4 PS(Pixel pixel) : SV_TARGET"
        "{"
        "    return float4(pixel.color, 1);"
        "}"
    );

    ComPtr<ID3DBlob> vertexShaderBlob = nullptr;
    CompileShader(shader, "VS", "vs_5_0", vertexShaderBlob.GetAddressOf());

    ComPtr<ID3D11VertexShader> vertexShader = nullptr;
    device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());

    ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
    CompileShader(shader, "PS", "ps_5_0", pixelShaderBlob.GetAddressOf());

    ComPtr<ID3D11PixelShader> pixelShader = nullptr;
    device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    ComPtr<ID3D11InputLayout> inputLayout = nullptr;
    device->CreateInputLayout(inputElementDesc.data(), (UINT)inputElementDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), inputLayout.GetAddressOf());

    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = sizeof(Constant);
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    ComPtr<ID3D11Buffer> constantBuffer = nullptr;
    device->CreateBuffer(&constantBufferDesc, nullptr, constantBuffer.GetAddressOf());

    Constant constant;

    constant.view = XMMatrixTranspose(
        XMMatrixInverse(
            nullptr,
            XMMatrixTranslation(0.0f, 0.0f, -5.0f)
        )
    );

    constant.projection = XMMatrixTranspose(
        XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), float(width) / height, 0.1f, 1000.0f)
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
            angle += 1.0f;
            constant.world = XMMatrixTranspose(
                XMMatrixRotationY(XMConvertToRadians(angle))
            );

            context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

            context->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &constant, 0, 0);
            context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

            UINT stride = sizeof(Vertex);
            UINT offset = 0;
            context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

            context->VSSetShader(vertexShader.Get(), nullptr, 0);
            context->PSSetShader(pixelShader.Get(), nullptr, 0);
            context->IASetInputLayout(inputLayout.Get());

            float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            context->ClearRenderTargetView(renderTargetView.Get(), clearColor);

            context->Draw((UINT)vertices.size(), 0);

            swapChain->Present(1, 0);
        }
    }

    return 0;
}
