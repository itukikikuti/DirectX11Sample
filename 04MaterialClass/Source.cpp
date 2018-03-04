#include "App.hpp"

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

	Material material(L"Shader.hlsl");
	material.SetBuffer(&constant, sizeof(constant));

	float angle = 0.0f;

	while (App::Refresh())
	{
		angle += 0.01f;
		constant.world = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixRotationY(angle)
		);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);

		material.Attach();

		App::GetGraphicsContext().Draw(vertices.size(), 0);
	}

	return 0;
}
