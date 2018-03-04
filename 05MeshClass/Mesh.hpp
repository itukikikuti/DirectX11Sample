struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
};

class Mesh
{
	PROTECTED struct Constant
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	PUBLIC std::vector<Vertex> vertices;
	PUBLIC Material material;
	PROTECTED Constant constant;
	PROTECTED ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;

	PUBLIC Mesh()
	{
		Initialize();
		CreateTriangle();
		Apply();
	}
	PUBLIC ~Mesh()
	{
	}
	PROTECTED void Initialize()
	{
		material = Material(L"Shader.hlsl");
	}
	PUBLIC void CreateTriangle()
	{
		vertices.clear();

		vertices.push_back({ DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) });
		vertices.push_back({ DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) });
		vertices.push_back({ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) });
	}
	PUBLIC void Apply()
	{
		if (vertices.size() > 0)
		{
			vertexBuffer.Release();
			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
			vertexSubresourceData.pSysMem = vertices.data();
			App::GetGraphicsDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
		}

		material.SetBuffer(&constant, sizeof(constant));
	}
	PUBLIC void Draw()
	{
		static float angle = 0.0f;

		angle += 0.01f;
		constant.world = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixRotationY(angle)
		);
		constant.view = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f)
		);
		constant.projection = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), App::GetWindowSize().x / (float)App::GetWindowSize().y, 0.1f, 100.0f)
		);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);

		material.Attach();

		App::GetGraphicsContext().Draw(vertices.size(), 0);
	}
};
