#pragma once
#include "Library.h"

class Camera
{
public:
    DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    Camera();
    void Start() const;
    void Stop() const;

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
};
