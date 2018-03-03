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
		CreateCube();
		Apply();
	}
	PUBLIC ~Mesh()
	{
	}
	PROTECTED void Initialize()
	{
		material = Material(L"Shader.hlsl");
	}
	PUBLIC void CreatePlane(Float2 size = Float2(0.5f, 0.5f), Float3 offset = Float3(0.0f, 0.0f, 0.0f), bool shouldClear = true, Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f), Float3 upDirection = Float3(0.0f, 1.0f, 0.0f), Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f))
	{
		if (shouldClear)
		{
			vertices.clear();
			indices.clear();
		}

		leftDirection = DirectX::XMVector3Normalize(leftDirection);
		upDirection = DirectX::XMVector3Normalize(upDirection);
		forwardDirection = DirectX::XMVector3Normalize(forwardDirection);

		vertices.push_back({ leftDirection * -size.x + upDirection * size.y + offset, -forwardDirection });
		vertices.push_back({ leftDirection * size.x + upDirection * size.y + offset, -forwardDirection });
		vertices.push_back({ leftDirection * -size.x + upDirection * -size.y + offset, -forwardDirection });
		vertices.push_back({ leftDirection * size.x + upDirection * -size.y + offset, -forwardDirection });

		size_t indexOffset = vertices.size() - 4;
		indices.push_back(indexOffset + 0);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 3);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 1);
	}
	PUBLIC void CreateCube(Float2 size = Float2(0.5f, 0.5f), Float3 offset = Float3(0.0f, 0.0f, 0.0f), bool shouldClear = true)
	{
		if (shouldClear)
		{
			vertices.clear();
			indices.clear();
		}

		CreatePlane(size, Float3(0.0f, 0.0f, -0.5f), false, Float3(1.0f, 0.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f), Float3(0.0f, 0.0f, 1.0f));	// front
		CreatePlane(size, Float3(0.0f, 0.0f, 0.5f), false, Float3(-1.0f, 0.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f), Float3(0.0f, 0.0f, -1.0f));	// back
		CreatePlane(size, Float3(0.5f, 0.0f, 0.0f), false, Float3(0.0f, 0.0f, 1.0f), Float3(0.0f, 1.0f, 0.0f), Float3(-1.0f, 0.0f, 0.0f));	// left
		CreatePlane(size, Float3(-0.5f, 0.0f, 0.0f), false, Float3(0.0f, 0.0f, -1.0f), Float3(0.0f, 1.0f, 0.0f), Float3(1.0f, 0.0f, 0.0f));	// right
		CreatePlane(size, Float3(0.0f, 0.5f, 0.0f), false, Float3(1.0f, 0.0f, 0.0f), Float3(0.0f, 0.0f, 1.0f), Float3(0.0f, -1.0f, 0.0f));	// up
		CreatePlane(size, Float3(0.0f, -0.5f, 0.0f), false, Float3(1.0f, 0.0f, 0.0f), Float3(0.0f, 0.0f, -1.0f), Float3(0.0f, 1.0f, 0.0f)); // down
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
		constant.world = DirectX::XMMatrixRotationY(angle);
		constant.view = DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f);
		constant.projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), App::GetWindowSize().x / (float)App::GetWindowSize().y, 0.1f, 100.0f);

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
