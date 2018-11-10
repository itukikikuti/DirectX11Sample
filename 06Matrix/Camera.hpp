// Camera.hpp
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

    Camera();
    void Start();
    void Stop();

private:
    struct ShaderData
    {
        DirectX::XMMATRIX viewMatrix;
        DirectX::XMMATRIX projectionMatrix;
    };

    CBuffer<ShaderData> shaderData;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};
