#pragma once

#define OEMRESOURCE
#include <memory>
#include <vector>
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define MAIN() APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)

class App final
{
public:
    static constexpr wchar_t* NAME = L"DirectX11";

#include "Window.hpp"

    App() = delete;
    static bool Refresh()
    {
        return GetWindow().Update();
    }
    static void Initialize()
    {
        static bool isInitialized = false;

        if (!isInitialized)
        {
            CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
            isInitialized = true;
        }
    }
    static HWND GetWindowHandle()
    {
        return GetWindow().GetHandle();
    }
    static DirectX::XMINT2 GetWindowSize()
    {
        return GetWindow().GetSize();
    }
    static void SetWindowSize(int width, int height)
    {
        return GetWindow().SetSize(width, height);
    }

private:
    static Window& GetWindow()
    {
        static std::unique_ptr<Window> window(new Window());
        return *window.get();
    }
};
