#pragma once
#include "Library.h"

class Camera
{
public:
    DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

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
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
};
