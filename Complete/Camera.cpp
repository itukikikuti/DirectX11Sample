#include "Window.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"

Camera::Camera()
{
    position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Graphics::GetSwapChain().GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(texture.GetAddressOf()));

    Graphics::GetDevice().CreateRenderTargetView(texture.Get(), nullptr, renderTargetView.GetAddressOf());

    cbuffer.Get().projectionMatrix = DirectX::XMMatrixTranspose(
        DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(60.0f),
            (float)Window::GetSize().x / (float)Window::GetSize().y,
            0.1f,
            1000.0f
        )
    );
}

void Camera::Start() const
{
    cbuffer.Get().viewMatrix = DirectX::XMMatrixTranspose(
        DirectX::XMMatrixInverse(
            nullptr,
            DirectX::XMMatrixRotationRollPitchYaw(
                DirectX::XMConvertToRadians(rotation.x),
                DirectX::XMConvertToRadians(rotation.y),
                DirectX::XMConvertToRadians(rotation.z)
            ) *
            DirectX::XMMatrixTranslation(position.x, position.y, position.z)
        )
    );

    cbuffer.Attach(1);

    Graphics::GetContext().OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

    float clearColor[4] = { color.x, color.y, color.z, color.w };
    Graphics::GetContext().ClearRenderTargetView(renderTargetView.Get(), clearColor);
}

void Camera::Stop() const
{
}
