class Graphics
{
	PRIVATE ATL::CComPtr<ID3D11Device> device = nullptr;
	PRIVATE ATL::CComPtr<IDXGISwapChain> swapChain = nullptr;
	PRIVATE ATL::CComPtr<ID3D11DeviceContext> context = nullptr;
	PRIVATE ATL::CComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;

	PUBLIC Graphics()
	{
		Initialize();
	}
	PUBLIC ~Graphics() {}
	PRIVATE void Initialize()
	{
		std::vector<D3D_FEATURE_LEVEL> featureLevels
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		UINT flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

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
		swapChainDesc.Flags = 0;
		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels.data(), featureLevels.size(), D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &context);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ATL::CComPtr<ID3D11Texture2D> renderTexture = nullptr;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&renderTexture));

		device->CreateRenderTargetView(renderTexture, nullptr, &renderTargetView);

		D3D11_VIEWPORT viewPort = {};
		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;
		viewPort.Width = static_cast<float>(App::GetWindowSize().x);
		viewPort.Height = static_cast<float>(App::GetWindowSize().y);
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		context->RSSetViewports(1, &viewPort);
	}
	PUBLIC ID3D11Device& GetDevice() const
	{
		return *device;
	}
	PUBLIC ID3D11DeviceContext& GetContext() const
	{
		return *context;
	}
	PUBLIC void Update()
	{
		swapChain->Present(1, 0);

		context->OMSetRenderTargets(1, &renderTargetView.p, nullptr);

		static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		context->ClearRenderTargetView(renderTargetView, color);
	}
};
