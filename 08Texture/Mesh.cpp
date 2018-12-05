// Mesh.cpp
#include "Graphics.hpp"
#include "Mesh.hpp"

Mesh::Mesh()
{
    position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

    CreateTriangle(1.0f);
}

void Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
{
    vertexSize = (UINT)indices.size();

    vertexBuffer.Reset();
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * (UINT)vertices.size();
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
    vertexSubresourceData.pSysMem = vertices.data();

    Graphics::GetDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());

    indexBuffer.Reset();
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = sizeof(UINT) * (UINT)indices.size();
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
    indexSubresourceData.pSysMem = indices.data();

    Graphics::GetDevice().CreateBuffer(&indexBufferDesc, &indexSubresourceData, indexBuffer.GetAddressOf());
}

void Mesh::CreateTriangle(float size)
{
    size *= 0.5f;

    std::vector<Vertex> vertices
    {
        Vertex(DirectX::XMFLOAT3(0.0f, size, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(size, -size, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
        Vertex(DirectX::XMFLOAT3(-size, -size, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f))
    };

    std::vector<UINT> indices
    {
        0, 1, 2
    };

    Create(vertices, indices);
}

void Mesh::CreatePlane(DirectX::XMFLOAT2 size)
{
    size.x *= 0.5f;
    size.y *= 0.5f;

    std::vector<Vertex> vertices
    {
        Vertex(DirectX::XMFLOAT3(-size.x, size.y, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, size.y, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, -size.y, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, -size.y, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f))
    };

    std::vector<UINT> indices
    {
        0, 1, 2,
        3, 2, 1
    };

    Create(vertices, indices);
}

void Mesh::CreateCube(DirectX::XMFLOAT3 size)
{
    size.x *= 0.5f;
    size.y *= 0.5f;
    size.z *= 0.5f;

    std::vector<Vertex> vertices
    {
        // 前
        Vertex(DirectX::XMFLOAT3(-size.x, size.y, -size.z), DirectX::XMFLOAT2(0.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, size.y, -size.z), DirectX::XMFLOAT2(1.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, -size.y, -size.z), DirectX::XMFLOAT2(0.0f, 1.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, -size.y, -size.z), DirectX::XMFLOAT2(1.0f, 1.0f)),

        // 後
        Vertex(DirectX::XMFLOAT3(size.x, size.y, size.z), DirectX::XMFLOAT2(0.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, size.y, size.z), DirectX::XMFLOAT2(1.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, -size.y, size.z), DirectX::XMFLOAT2(0.0f, 1.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, -size.y, size.z), DirectX::XMFLOAT2(1.0f, 1.0f)),

        // 左
        Vertex(DirectX::XMFLOAT3(size.x, size.y, -size.z), DirectX::XMFLOAT2(0.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, size.y, size.z), DirectX::XMFLOAT2(1.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, -size.y, -size.z), DirectX::XMFLOAT2(0.0f, 1.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, -size.y, size.z), DirectX::XMFLOAT2(1.0f, 1.0f)),

        // 右
        Vertex(DirectX::XMFLOAT3(-size.x, size.y, size.z), DirectX::XMFLOAT2(0.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, size.y, -size.z), DirectX::XMFLOAT2(1.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, -size.y, size.z), DirectX::XMFLOAT2(0.0f, 1.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, -size.y, -size.z), DirectX::XMFLOAT2(1.0f, 1.0f)),

        // 上
        Vertex(DirectX::XMFLOAT3(-size.x, size.y, size.z), DirectX::XMFLOAT2(0.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, size.y, size.z), DirectX::XMFLOAT2(1.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, size.y, -size.z), DirectX::XMFLOAT2(0.0f, 1.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, size.y, -size.z), DirectX::XMFLOAT2(1.0f, 1.0f)),

        // 下
        Vertex(DirectX::XMFLOAT3(-size.x, -size.y, -size.z), DirectX::XMFLOAT2(0.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, -size.y, -size.z), DirectX::XMFLOAT2(1.0f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(-size.x, -size.y, size.z), DirectX::XMFLOAT2(0.0f, 1.0f)),
        Vertex(DirectX::XMFLOAT3(size.x, -size.y, size.z), DirectX::XMFLOAT2(1.0f, 1.0f))
    };

    std::vector<UINT> indices
    {
        // 前
        0, 1, 2,
        3, 2, 1,

        // 後
        4, 5, 6,
        7, 6, 5,

        // 左
        8, 9, 10,
        11, 10, 9,

        // 右
        12, 13, 14,
        15, 14, 13,

        // 上
        16, 17, 18,
        19, 18, 17,

        // 下
        20, 21, 22,
        23, 22, 21
    };

    Create(vertices, indices);
}

void Mesh::Draw()
{
    cbuffer.Get().modelMatrix = DirectX::XMMatrixTranspose(
        DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
        DirectX::XMMatrixRotationRollPitchYaw(
            DirectX::XMConvertToRadians(rotation.x),
            DirectX::XMConvertToRadians(rotation.y),
            DirectX::XMConvertToRadians(rotation.z)
        ) *
        DirectX::XMMatrixTranslation(position.x, position.y, position.z)
    );

    cbuffer.Attach(0);
    texture.Attach(0);
    shader.Attach();

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    Graphics::GetContext().IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    Graphics::GetContext().IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    Graphics::GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Graphics::GetContext().DrawIndexed(vertexSize, 0, 0);
}
