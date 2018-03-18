class Mesh
{
public:
    float angle = 0.0f;
    std::vector<Vertex> vertices;

    Mesh()
    {
        App::Initialize();

        material = Material(L"Shader.hlsl");

        CreateTriangle();
        Apply();
    }
    ~Mesh()
    {
    }
    void CreateTriangle()
    {
        vertices.clear();

        vertices.push_back({ DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) });
        vertices.push_back({ DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) });
        vertices.push_back({ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) });
    }
    void Apply()
    {
        if (vertices.size() > 0)
        {
            D3D11_BUFFER_DESC vertexBufferDesc = {};
            vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
            vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
            vertexSubresourceData.pSysMem = vertices.data();

            vertexBuffer.Release();
            App::GetGraphicsDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
        }

        material.SetBuffer(&constant, sizeof(Constant));
    }
    void Draw()
    {
        if (vertexBuffer == nullptr)
            return;

        constant.world = DirectX::XMMatrixTranspose(
            DirectX::XMMatrixRotationY(angle)
        );
        constant.view = DirectX::XMMatrixTranspose(
            DirectX::XMMatrixTranslation(0.0f, 0.0f, 5.0f)
        );
        constant.projection = DirectX::XMMatrixTranspose(
            DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)App::GetWindowSize().x / App::GetWindowSize().y, 0.1f, 100.0f)
        );

        material.Attach();

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);

        App::GetGraphicsContext().Draw(vertices.size(), 0);
    }

private:
    struct Constant
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    Material material;
    Constant constant;
    ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
};