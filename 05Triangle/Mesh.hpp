// Mesh.hpp
#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Shader.hpp"

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 color;

    Vertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color)
        : position(position), color(color)
    {
    }
};

class Mesh
{
public:
    Shader shader = Shader::GetDefault();

    Mesh();
    void Draw();

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    UINT vertexSize;
};
