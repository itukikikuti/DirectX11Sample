#pragma once
#include "Library.h"

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 uv;

    Vertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 uv)
    {
        this->position = position;
        this->uv = uv;
    }
};

class Mesh
{
public:
    DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    Texture& texture = Texture::GetEmpty();
    Shader& shader = Shader::GetDefault();

    Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);
    void Draw() const;

    static std::unique_ptr<Mesh> CreateTriangle();
    static std::unique_ptr<Mesh> CreatePlane(DirectX::XMFLOAT2 size);
    static std::unique_ptr<Mesh> CreateCube(DirectX::XMFLOAT3 size);

private:
    struct ShaderVariable
    {
        DirectX::XMFLOAT4X4 modelMatrix;
    };

    CBuffer<ShaderVariable> cbuffer;
    UINT vertexCount;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer = nullptr;
};
