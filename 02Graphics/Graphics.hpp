// Graphics.hpp
#pragma once
#include <d3d11.h>
#include <wrl.h>

class Graphics
{
public:
    static void Initialize();
    static ID3D11Device& GetDevice();
    static ID3D11DeviceContext& GetContext();
    static IDXGISwapChain& GetSwapChain();
    static void Update();

private:
    static Microsoft::WRL::ComPtr<ID3D11Device> device;
    static Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    static Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
};
