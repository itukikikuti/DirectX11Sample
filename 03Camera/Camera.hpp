// Camera.hpp
#pragma once
#include <d3d11.h>
#include <wrl.h>

class Camera
{
public:
    Camera();
    ~Camera();
    void Update();

private:
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};
