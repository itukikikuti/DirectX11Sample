#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

class Camera
{
public:
    DirectX::XMFLOAT4 color;

    Camera();
    void Start() const;
    void Stop() const;

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};
