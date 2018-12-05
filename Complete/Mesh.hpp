#pragma once
#include <memory>
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
    {
        this->position = position;
        this->uv = uv;
    }
};

class Mesh
{
public:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scale;
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
        DirectX::XMMATRIX modelMatrix;
    };

    CBuffer<ShaderVariable> cbuffer;
    UINT vertexCount;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};
