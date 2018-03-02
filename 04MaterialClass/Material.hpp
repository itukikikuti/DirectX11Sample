class Material
{
	PROTECTED void* buffer = nullptr;
	PROTECTED ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;
	PROTECTED ATL::CComPtr<ID3D11VertexShader> vertexShader = nullptr;
	PROTECTED ATL::CComPtr<ID3D11PixelShader> pixelShader = nullptr;
	PROTECTED ATL::CComPtr<ID3D11InputLayout> inputLayout = nullptr;

	PUBLIC Material() {}
	PUBLIC Material(const wchar_t* const filePath)
	{
		Load(filePath);
	}
	PUBLIC ~Material() {}
	PROTECTED void Create(const wchar_t* const filePath)
	{
		vertexShader.Release();
		ATL::CComPtr<ID3DBlob> vertexShaderBlob = nullptr;
		CompileShader(filePath, "VS", "vs_5_0", &vertexShaderBlob);
		App::GetGraphicsDevice().CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);

		pixelShader.Release();
		ATL::CComPtr<ID3DBlob> pixelShaderBlob = nullptr;
		CompileShader(filePath, "PS", "ps_5_0", &pixelShaderBlob);
		App::GetGraphicsDevice().CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);

		inputLayout.Release();
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		App::GetGraphicsDevice().CreateInputLayout(inputElementDesc.data(), inputElementDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
	}
	PUBLIC void Load(const wchar_t* const filePath)
	{
		Create(filePath);
	}
	PUBLIC void SetBuffer(void* cbuffer, size_t size)
	{
		buffer = cbuffer;

		constantBuffer.Release();
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.ByteWidth = size;
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;
		App::GetGraphicsDevice().CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
	}
	PUBLIC void Attach()
	{
		if (buffer != nullptr)
		{
			App::GetGraphicsContext().UpdateSubresource(constantBuffer, 0, nullptr, buffer, 0, 0);
			App::GetGraphicsContext().VSSetConstantBuffers(0, 1, &constantBuffer.p);
		}

		App::GetGraphicsContext().VSSetShader(vertexShader, nullptr, 0);
		App::GetGraphicsContext().PSSetShader(pixelShader, nullptr, 0);
		App::GetGraphicsContext().IASetInputLayout(inputLayout);
	}
	PROTECTED void CompileShader(const wchar_t* const filePath, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out)
	{
		DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG;
#endif

		ATL::CComPtr<ID3DBlob> errorBlob = nullptr;
		D3DCompileFromFile(filePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, flags, 0, out, &errorBlob);

		if (errorBlob != nullptr)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			MessageBoxA(App::GetWindowHandle(), (char*)errorBlob->GetBufferPointer(), "Shader Error", MB_OK);
		}
	}
};
