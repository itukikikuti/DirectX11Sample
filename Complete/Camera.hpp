#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Cbuffer.hpp"

class Camera
{
public:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT4 color;

    Camera();
    void Start() const;
    void Stop() const;

private:
    struct ShaderVariable
    {
        DirectX::XMMATRIX viewMatrix;
        DirectX::XMMATRIX projectionMatrix;
    };

    CBuffer<ShaderVariable> cbuffer;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};
