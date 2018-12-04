// Graphics.hpp
#pragma once
#include <d3d11.h>
#include <wincodec.h>
#include <wrl.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "windowscodecs.lib")

class Graphics
{
public:
    static void Initialize();
    static ID3D11Device& GetDevice();
    static ID3D11DeviceContext& GetContext();
    static IDXGISwapChain& GetSwapChain();
    static IWICImagingFactory& GetImageFactory();
    static void Update();

private:
    static Microsoft::WRL::ComPtr<ID3D11Device> device;
    static Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    static Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    static Microsoft::WRL::ComPtr<IWICImagingFactory> imageFactory;
};
