class Graphics
{
public:
	Graphics()
	{
        App::Initialize();

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

        UINT flags = 0;
#if defined(_DEBUG)
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Width = App::GetWindowSize().x;
        swapChainDesc.BufferDesc.Height = App::GetWindowSize().y;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.OutputWindow = App::GetWindowHandle();
        swapChainDesc.Windowed = true;

        for (size_t i = 0; i < driverTypes.size(); i++)
        {
            HRESULT r = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i], nullptr, flags, featureLevels.data(), (UINT)featureLevels.size(), D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &context);

            if (SUCCEEDED(r))
                break;
        }

        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        ATL::CComPtr<ID3D11Texture2D> renderTexture = nullptr;
        swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&renderTexture));

        device->CreateRenderTargetView(renderTexture, nullptr, &renderTargetView);

        D3D11_VIEWPORT viewPort = {};
        viewPort.Width = static_cast<float>(App::GetWindowSize().x);
        viewPort.Height = static_cast<float>(App::GetWindowSize().y);
        viewPort.MaxDepth = 1.0f;
        context->RSSetViewports(1, &viewPort);
	}
	~Graphics()
	{
	}
	ID3D11Device& GetDevice() const
	{
		return *device;
	}
	ID3D11DeviceContext& GetContext() const
	{
		return *context;
	}
	void Update()
	{
		swapChain->Present(1, 0);

		context->OMSetRenderTargets(1, &renderTargetView.p, nullptr);

		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		context->ClearRenderTargetView(renderTargetView, color);
	}

private:
    ATL::CComPtr<ID3D11Device> device = nullptr;
    ATL::CComPtr<IDXGISwapChain> swapChain = nullptr;
    ATL::CComPtr<ID3D11DeviceContext> context = nullptr;
    ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
};