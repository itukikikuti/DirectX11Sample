#pragma once
#include "Library.h"

struct Vertex
{
    DirectX::XMFLOAT3 position;

    Vertex(DirectX::XMFLOAT3 position)
    {
        this->position = position;
    }
};

class Mesh
{
public:
    DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    Shader& shader = Shader::GetDefault();

    Mesh(const std::vector<Vertex>& vertices);
    void Draw() const;

    static std::unique_ptr<Mesh> CreateTriangle();

private:
    struct ShaderVariable
    {
        DirectX::XMFLOAT4X4 modelMatrix;
    };

    CBuffer<ShaderVariable> cbuffer;
    UINT vertexCount;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
};
