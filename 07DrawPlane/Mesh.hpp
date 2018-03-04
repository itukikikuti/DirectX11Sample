class Mesh
{
	PROTECTED struct Constant
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	PUBLIC std::vector<Vertex> vertices;
	PUBLIC std::vector<int> indices;
	PUBLIC Material material;
	PROTECTED Constant constant;
	PROTECTED ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	PROTECTED ATL::CComPtr<ID3D11Buffer> indexBuffer = nullptr;

	PUBLIC Mesh()
	{
		Initialize();
		CreatePlane();
		Apply();
	}
	PUBLIC ~Mesh()
	{
	}
	PROTECTED void Initialize()
	{
		material = Material(L"Shader.hlsl");
	}
	PUBLIC void CreatePlane()
	{
		vertices.clear();
		indices.clear();

		vertices.push_back({ Float3(-0.5f, 0.5f, 0.0f), Float3(1.0f, 0.0f, 0.0f) });
		vertices.push_back({ Float3(0.5f, 0.5f, 0.0f), Float3(0.0f, 1.0f, 0.0f) });
		vertices.push_back({ Float3(-0.5f, -0.5f, 0.0f), Float3(0.0f, 0.0f, 1.0f) });
		vertices.push_back({ Float3(0.5f, -0.5f, 0.0f), Float3(1.0f, 1.0f, 1.0f) });

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(1);
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

		if (indices.size() > 0) {
			indexBuffer.Release();
			D3D11_BUFFER_DESC indexBufferDesc = {};
			indexBufferDesc.ByteWidth = sizeof(int) * indices.size();
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
			indexSubresourceData.pSysMem = indices.data();
			App::GetGraphicsDevice().CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
		}

		material.SetBuffer(&constant, sizeof(constant));
	}
	PUBLIC void Draw()
	{
		if (vertexBuffer == nullptr) return;

		static float angle = 0.0f;

		angle += 0.01f;
		constant.world = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixRotationY(angle)
		);
		constant.view = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f)
		);
		constant.projection = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), App::GetWindowSize().x / (float)App::GetWindowSize().y, 0.1f, 100.0f)
		);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);

		material.Attach();

		if (indexBuffer == nullptr)
		{
			App::GetGraphicsContext().Draw(vertices.size(), 0);
		}
		else
		{
			App::GetGraphicsContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			App::GetGraphicsContext().DrawIndexed(indices.size(), 0, 0);
		}
	}
};
