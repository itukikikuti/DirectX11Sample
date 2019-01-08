#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Cbuffer.h"

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
        DirectX::XMFLOAT4X4 viewMatrix;
        DirectX::XMFLOAT4X4 projectionMatrix;
    };

    CBuffer<ShaderVariable> cbuffer;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};
