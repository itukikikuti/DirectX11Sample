class Camera
{
public:
    Float3 position;
    Float3 angles;
    Float4 color;
    bool isDepthTest;

    Camera()
    {
        App::Initialize();

        position = Float3(0.0f, 0.0f, 0.0f);
        angles = Float3(0.0f, 0.0f, 0.0f);
        color = Float4(1.0f, 1.0f, 1.0f, 1.0);
        isDepthTest = true;

        Create();
    }
    ~Camera()
    {
    }
    void Update()
    {
        constant.view = DirectX::XMMatrixTranspose(
            DirectX::XMMatrixInverse(
                nullptr,
                DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x)) *
                DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y)) *
                DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z)) *
                DirectX::XMMatrixTranslation(position.x, position.y, position.z)
            )
        );

        constant.projection = DirectX::XMMatrixTranspose(
            DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), App::GetWindowSize().x / (float)App::GetWindowSize().y, 0.1f, 100.0f)
        );

        App::GetGraphicsContext().UpdateSubresource(constantBuffer, 0, nullptr, &constant, 0, 0);
        App::GetGraphicsContext().VSSetConstantBuffers(0, 1, &constantBuffer.p);
        App::GetGraphicsContext().HSSetConstantBuffers(0, 1, &constantBuffer.p);
        App::GetGraphicsContext().DSSetConstantBuffers(0, 1, &constantBuffer.p);
        App::GetGraphicsContext().GSSetConstantBuffers(0, 1, &constantBuffer.p);
        App::GetGraphicsContext().PSSetConstantBuffers(0, 1, &constantBuffer.p);

        float clearColor[4] = { color.x, color.y, color.z, color.w };
        App::GetGraphicsContext().ClearRenderTargetView(renderTargetView, clearColor);

        if (isDepthTest)
        {
            App::GetGraphicsContext().ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            App::GetGraphicsContext().OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
        }
        else
        {
            App::GetGraphicsContext().OMSetRenderTargets(1, &renderTargetView.p, nullptr);
        }
    }
private:
    struct Constant
    {
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    Constant constant;
    ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
    ATL::CComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
    ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;

    void Create()
    {
        ATL::CComPtr<ID3D11Texture2D> renderTexture = nullptr;
        App::GetGraphicsSwapChain().GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&renderTexture));

        renderTargetView.Release();
        App::GetGraphicsDevice().CreateRenderTargetView(renderTexture, nullptr, &renderTargetView);

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = static_cast<UINT>(App::GetWindowSize().x);
        textureDesc.Height = static_cast<UINT>(App::GetWindowSize().y);
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        ATL::CComPtr<ID3D11Texture2D> depthTexture = nullptr;
        App::GetGraphicsDevice().CreateTexture2D(&textureDesc, nullptr, &depthTexture);

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
        depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

        depthStencilView.Release();
        App::GetGraphicsDevice().CreateDepthStencilView(depthTexture, &depthStencilViewDesc, &depthStencilView);

        constantBuffer.Release();
        D3D11_BUFFER_DESC constantBufferDesc = {};
        constantBufferDesc.ByteWidth = sizeof(Constant);
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        App::GetGraphicsDevice().CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
    }
};