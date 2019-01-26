﻿#pragma once
#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include "Graphics.h"

template <class T>
class CBuffer
{
public:
    CBuffer()
    {
        instance = std::make_unique<T>();

        D3D11_BUFFER_DESC cbufferDesc = {};
        cbufferDesc.ByteWidth = sizeof(T);
        cbufferDesc.Usage = D3D11_USAGE_DEFAULT;
        cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        Graphics::GetDevice().CreateBuffer(&cbufferDesc, nullptr, buffer.GetAddressOf());
    }

    T& Get() const
    {
        return *instance.get();
    }

    void Attach(int slot) const
    {
        Graphics::GetContext().UpdateSubresource(buffer.Get(), 0, nullptr, instance.get(), 0, 0);
        Graphics::GetContext().VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
        Graphics::GetContext().PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
    }

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    std::unique_ptr<T> instance;
};