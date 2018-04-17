class Mesh
{
public:
    float angle = 0.0f;
    std::vector<Vertex> vertices;
    std::vector<UINT> indices;

    Mesh()
    {
        App::Initialize();

        material = Material(
            "cbuffer Constant : register(b0)"
            "{"
            "    matrix world;"
            "    matrix view;"
            "    matrix projection;"
            "};"
            "struct Vertex"
            "{"
            "    float4 position : POSITION;"
            "    float3 color : COLOR;"
            "};"
            "struct Pixel"
            "{"
            "    float4 position : SV_POSITION;"
            "    float3 color : COLOR;"
            "};"
            "Pixel VS(Vertex vertex)"
            "{"
            "    Pixel output;"
            "    output.position = mul(vertex.position, world);"
            "    output.position = mul(output.position, view);"
            "    output.position = mul(output.position, projection);"
            "    output.color = vertex.color;"
            "    return output;"
            "}"
            "float4 PS(Pixel pixel) : SV_TARGET"
            "{"
            "    return float4(pixel.color, 1);"
            "}"
        );

        CreateCube();
        Apply();
    }
    ~Mesh()
    {
    }
    void CreateTriangle(bool shouldClear = true)
    {
        if (shouldClear)
        {
            vertices.clear();
            indices.clear();
        }

        vertices.push_back({ Float3(0.0f, 1.0f, 0.0f), Float3(1.0f, 0.0f, 0.0f) });
        vertices.push_back({ Float3(1.0f, -1.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f) });
        vertices.push_back({ Float3(-1.0f, -1.0f, 0.0f), Float3(0.0f, 0.0f, 1.0f) });
    }
    void CreatePlane(Float2 size = Float2(0.5f, 0.5f), Float3 offset = Float3(0.0f, 0.0f, 0.0f), bool shouldClear = true, Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f), Float3 upDirection = Float3(0.0f, 1.0f, 0.0f))
    {
        if (shouldClear)
        {
            vertices.clear();
            indices.clear();
        }

        leftDirection = DirectX::XMVector3Normalize(leftDirection);
        upDirection = DirectX::XMVector3Normalize(upDirection);

        vertices.push_back({ leftDirection * -size.x + upDirection * size.y + offset, Float3(1.0f, 0.0f, 0.0f) });
        vertices.push_back({ leftDirection * size.x + upDirection * size.y + offset, Float3(1.0f, 1.0f, 0.0f) });
        vertices.push_back({ leftDirection * -size.x + upDirection * -size.y + offset, Float3(0.0f, 0.0f, 1.0f) });
        vertices.push_back({ leftDirection * size.x + upDirection * -size.y + offset, Float3(0.0f, 1.0f, 0.0f) });

        UINT indexOffset = (UINT)vertices.size() - 4;
        indices.push_back(indexOffset + 0);
        indices.push_back(indexOffset + 1);
        indices.push_back(indexOffset + 2);
        indices.push_back(indexOffset + 3);
        indices.push_back(indexOffset + 2);
        indices.push_back(indexOffset + 1);
    }
    void CreateCube(Float2 size = Float2(0.5f, 0.5f), bool shouldClear = true)
    {
        if (shouldClear)
        {
            vertices.clear();
            indices.clear();
        }

        CreatePlane(size, Float3(0.0f, 0.0f, -0.5f), false, Float3(1.0f, 0.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f));    // front
        CreatePlane(size, Float3(0.0f, 0.0f, 0.5f), false, Float3(-1.0f, 0.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f));    // back
        CreatePlane(size, Float3(0.5f, 0.0f, 0.0f), false, Float3(0.0f, 0.0f, 1.0f), Float3(0.0f, 1.0f, 0.0f));     // left
        CreatePlane(size, Float3(-0.5f, 0.0f, 0.0f), false, Float3(0.0f, 0.0f, -1.0f), Float3(0.0f, 1.0f, 0.0f));   // right
        CreatePlane(size, Float3(0.0f, 0.5f, 0.0f), false, Float3(1.0f, 0.0f, 0.0f), Float3(0.0f, 0.0f, 1.0f));     // up
        CreatePlane(size, Float3(0.0f, -0.5f, 0.0f), false, Float3(1.0f, 0.0f, 0.0f), Float3(0.0f, 0.0f, -1.0f));   // down
    }
    void Apply()
    {
        if (vertices.size() > 0)
        {
            D3D11_BUFFER_DESC vertexBufferDesc = {};
            vertexBufferDesc.ByteWidth = sizeof(Vertex) * (UINT)vertices.size();
            vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
            vertexSubresourceData.pSysMem = vertices.data();

            vertexBuffer.Release();
            App::GetGraphicsDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
        }

        if (indices.size() > 0)
        {
            D3D11_BUFFER_DESC indexBufferDesc = {};
            indexBufferDesc.ByteWidth = sizeof(UINT) * (UINT)indices.size();
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

            D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
            indexSubresourceData.pSysMem = indices.data();

            indexBuffer.Release();
            App::GetGraphicsDevice().CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
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
            DirectX::XMMatrixLookAtLH(Float3(0.0f, 1.0f, -3.0f), Float3(0.0f, 0.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f))
        );
        constant.projection = DirectX::XMMatrixTranspose(
            DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), App::GetWindowSize().x / (float)App::GetWindowSize().y, 0.1f, 100.0f)
        );

        material.Attach();

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p, &stride, &offset);

        if (indexBuffer == nullptr)
        {
            App::GetGraphicsContext().Draw((UINT)vertices.size(), 0);
        }
        else
        {
            App::GetGraphicsContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
            App::GetGraphicsContext().DrawIndexed((UINT)indices.size(), 0, 0);
        }
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
    ATL::CComPtr<ID3D11Buffer> indexBuffer = nullptr;
};