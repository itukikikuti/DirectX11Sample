#include "App.hpp"

void CompileShader(const wchar_t* const filePath, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out);

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

int Main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::vector<Vertex> vertices
	{
		{ DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};
	ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
	vertexSubresourceData.pSysMem = vertices.data();
	App::GetGraphicsDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);

	ATL::CComPtr<ID3D11VertexShader> vertexShader = nullptr;
	ATL::CComPtr<ID3DBlob> vertexShaderBlob = nullptr;
	CompileShader(L"Shader.hlsl", "VS", "vs_5_0", &vertexShaderBlob);
	App::GetGraphicsDevice().CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);

	ATL::CComPtr<ID3D11PixelShader> pixelShader = nullptr;
	ATL::CComPtr<ID3DBlob> pixelShaderBlob = nullptr;
	CompileShader(L"Shader.hlsl", "PS", "ps_5_0", &pixelShaderBlob);
	App::GetGraphicsDevice().CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);

	ATL::CComPtr<ID3D11InputLayout> inputLayout = nullptr;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	App::GetGraphicsDevice().CreateInputLayout(inputElementDesc.data(), inputElementDesc.size(), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);

	ATL::CComPtr<ID3D11Buffer> constantBuffer = nullptr;
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(Constant);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	App::GetGraphicsDevice().CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);

	Constant constant;
	constant.world = DirectX::XMMatrixTranspose
	(
		DirectX::XMMatrixIdentity()
	);
	constant.view = DirectX::XMMatrixTranspose
	(
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f)
	);
	constant.projection = DirectX::XMMatrixTranspose
	(
		DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), App::GetWindowSize().x / (float)App::GetWindowSize().y, 0.1f, 100.0f)
	);

	float angle = 0.0f;

	while (App::Refresh())
	{
		angle += 0.01f;
		constant.world = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixRotationY(angle)
		);

		App::GetGraphicsContext().UpdateSubresource(constantBuffer, 0, nullptr, &constant, 0, 0);
		App::GetGraphicsContext().VSSetConstantBuffers(0, 1, &constantBuffer.p);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);

		App::GetGraphicsContext().VSSetShader(vertexShader, nullptr, 0);
		App::GetGraphicsContext().PSSetShader(pixelShader, nullptr, 0);
		App::GetGraphicsContext().IASetInputLayout(inputLayout);

		App::GetGraphicsContext().Draw(vertices.size(), 0);
	}

	return 0;
}

void CompileShader(const wchar_t* const filePath, const char* const entryPoint, const char* const shaderModel, ID3DBlob** out)
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
