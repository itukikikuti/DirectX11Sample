// Mesh.hpp
#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Cbuffer.hpp"
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
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scale;
    Shader shader = Shader::GetDefault();

    Mesh();
    void Draw();

private:
    struct ShaderData
    {
        DirectX::XMMATRIX modelMatrix;
    };

    CBuffer<ShaderData> cbuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    UINT vertexSize;
};
