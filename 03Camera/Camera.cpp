// Camera.cpp
#include "Window.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"

Camera::Camera()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
    Graphics::GetSwapChain().GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(texture.GetAddressOf()));

    Graphics::GetDevice().CreateRenderTargetView(texture.Get(), nullptr, renderTargetView.GetAddressOf());

    D3D11_VIEWPORT viewPort = {};
    viewPort.Width = (float)Window::GetSize().x;
    viewPort.Height = (float)Window::GetSize().y;
    viewPort.MaxDepth = 1.0f;
    Graphics::GetContext().RSSetViewports(1, &viewPort);
}

void Camera::Start()
{
    Graphics::GetContext().OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

    float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    Graphics::GetContext().ClearRenderTargetView(renderTargetView.Get(), clearColor);
}

void Camera::Stop()
{
}
