#include "Library.h"

Camera::Camera()
{
    InitializeLibrary();

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
    Graphics::GetSwapChain().GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(texture.GetAddressOf()));

    Graphics::GetDevice().CreateRenderTargetView(texture.Get(), nullptr, renderTargetView.GetAddressOf());
}

void Camera::Start() const
{
    Graphics::GetContext().OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

    float clearColor[4] = { color.x, color.y, color.z, color.w };
    Graphics::GetContext().ClearRenderTargetView(renderTargetView.Get(), clearColor);
}

void Camera::Stop() const
{
}
