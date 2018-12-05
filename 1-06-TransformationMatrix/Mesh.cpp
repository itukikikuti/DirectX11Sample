#include "Graphics.hpp"
#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices)
{
    position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

    vertexCount = (UINT)vertices.size();

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * (UINT)vertices.size();
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
    vertexSubresourceData.pSysMem = vertices.data();

    Graphics::GetDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, vertexBuffer.GetAddressOf());
}

void Mesh::Draw() const
{
    DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixTranspose(
        DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
        DirectX::XMMatrixRotationRollPitchYaw(
            DirectX::XMConvertToRadians(rotation.x),
            DirectX::XMConvertToRadians(rotation.y),
            DirectX::XMConvertToRadians(rotation.z)
        ) *
        DirectX::XMMatrixTranslation(position.x, position.y, position.z)
    );

    DirectX::XMStoreFloat4x4(&cbuffer.Get().modelMatrix, modelMatrix);

    cbuffer.Attach(0);
    shader.Attach();

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    Graphics::GetContext().IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    Graphics::GetContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Graphics::GetContext().Draw(vertexCount, 0);
}

std::unique_ptr<Mesh> Mesh::CreateTriangle()
{
    std::vector<Vertex> vertices
    {
        Vertex(DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f)),
        Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f))
    };

    return std::make_unique<Mesh>(vertices);
}
