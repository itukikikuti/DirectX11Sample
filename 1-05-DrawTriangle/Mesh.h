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
    Shader& shader = Shader::GetDefault();

    Mesh(const std::vector<Vertex>& vertices);
    void Draw() const;

    static std::unique_ptr<Mesh> CreateTriangle();

private:
    UINT vertexCount;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
};
