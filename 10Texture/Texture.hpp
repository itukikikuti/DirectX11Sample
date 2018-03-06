class Texture
{
	PROTECTED DirectX::XMINT2 size;
	PROTECTED ATL::CComPtr<ID3D11Texture2D> texture;
	PROTECTED ATL::CComPtr<ID3D11ShaderResourceView> shaderResourceView;
	PROTECTED ATL::CComPtr<ID3D11SamplerState> samplerState;

	PUBLIC Texture()
	{
		Initialize();
	}
	PUBLIC Texture(const wchar_t* const filePath)
	{
		Initialize();
		Load(filePath);
	}
	PUBLIC ~Texture()
	{
	}
	PROTECTED void Initialize()
	{
		App::Initialize();
	}
	PROTECTED void Create(const wchar_t* const filePath)
	{
		static ATL::CComPtr<IWICImagingFactory> factory = nullptr;
		if (factory == nullptr)
		{
			CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
		}

		ATL::CComPtr<IWICBitmapDecoder> decoder = nullptr;
		factory->CreateDecoderFromFilename(filePath, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

		ATL::CComPtr<IWICBitmapFrameDecode> frame = nullptr;
		decoder->GetFrame(0, &frame);
		UINT width, height;
		frame->GetSize(&width, &height);
		size = DirectX::XMINT2(width, height);

		WICPixelFormatGUID pixelFormat;
		frame->GetPixelFormat(&pixelFormat);
		std::unique_ptr<BYTE[]> buffer(new BYTE[width * height * 4]);

		if (pixelFormat != GUID_WICPixelFormat32bppRGBA)
		{
			ATL::CComPtr<IWICFormatConverter> formatConverter = nullptr;
			factory->CreateFormatConverter(&formatConverter);

			formatConverter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
			formatConverter->CopyPixels(0, width * 4, width * height * 4, buffer.get());
		}
		else
		{
			frame->CopyPixels(0, width * 4, width * height * 4, buffer.get());
		}

		texture.Release();
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
		textureSubresourceData.pSysMem = buffer.get();
		textureSubresourceData.SysMemPitch = width * 4;
		textureSubresourceData.SysMemSlicePitch = width * height * 4;
		App::GetGraphicsDevice().CreateTexture2D(&textureDesc, &textureSubresourceData, &texture);

		shaderResourceView.Release();
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		App::GetGraphicsDevice().CreateShaderResourceView(texture, &shaderResourceViewDesc, &shaderResourceView);

		samplerState.Release();
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		App::GetGraphicsDevice().CreateSamplerState(&samplerDesc, &samplerState);
	}
	PUBLIC void Load(const wchar_t* const filePath)
	{
		Create(filePath);
	}
	PUBLIC DirectX::XMINT2 GetSize()
	{
		return size;
	}
	PUBLIC void Attach(int slot)
	{
		App::GetGraphicsContext().PSSetShaderResources(slot, 1, &shaderResourceView.p);
		App::GetGraphicsContext().PSSetSamplers(slot, 1, &samplerState.p);
	}
};
