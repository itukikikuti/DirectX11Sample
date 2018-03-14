#include "App.hpp"

using namespace std;
using namespace ATL;
using namespace DirectX;

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
        MessageBoxA(
            App::GetWindowHandle(),
            static_cast<char*>(errorBlob->GetBufferPointer()),
            "Shader Error",
            MB_OK
        );
    }
}

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 color;
};

struct Constant
{
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
};

int MAIN()
{
    vector<D3D_FEATURE_LEVEL> featureLevels
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = App::GetWindowSize().x;
    swapChainDesc.BufferDesc.Height = App::GetWindowSize().y;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = App::GetWindowHandle();
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
    viewPort.Width = App::GetWindowSize().x;
    viewPort.Height = App::GetWindowSize().y;
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
            (float)App::GetWindowSize().x / App::GetWindowSize().y,
            0.1f,
            100.0f
        )
    );

    float angle = 0.0f;

    while (App::Refresh())
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

    return 0;
}
