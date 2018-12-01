// Camera.hpp
#pragma once
#include <d3d11.h>
#include <wrl.h>

class Camera
{
public:
    Camera();
    void Start();
    void Stop();

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};
