// Mesh.hpp
#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Cbuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 uv;

    Vertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 uv)
        : position(position), uv(uv)
    {
    }
};

class Mesh
{
public:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scale;
    Shader& shader = Shader::GetDefault();
    Texture& texture = Texture::GetEmpty();

    Mesh();
    void Create(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices);
    void CreateTriangle(float size);
    void CreatePlane(DirectX::XMFLOAT2 size);
    void CreateCube(DirectX::XMFLOAT3 size);
    void Draw();

private:
    struct ShaderData
    {
        DirectX::XMMATRIX modelMatrix;
    };

    CBuffer<ShaderData> cbuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    UINT vertexSize;
};
