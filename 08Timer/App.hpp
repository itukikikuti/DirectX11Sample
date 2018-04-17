#pragma once

#define OEMRESOURCE
#include <memory>
#include <string>
#include <vector>
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wincodec.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define MAIN() APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)

#include "Utility.hpp"

class App final
{
public:
#include "Window.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Timer.hpp"

	App() = delete;
	static bool Refresh()
	{
		GetGraphics().Update();
        GetInput().Update();
        GetTimer().Update();
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
	static ID3D11Device& GetGraphicsDevice()
	{
		return GetGraphics().GetDevice();
	}
    static IDXGISwapChain& GetGraphicsSwapChain()
    {
        return GetGraphics().GetSwapChain();
    }
	static ID3D11DeviceContext& GetGraphicsContext()
	{
		return GetGraphics().GetContext();
	}
    static bool GetKey(int VK_CODE)
    {
        return GetInput().GetKey(VK_CODE);
    }
    static bool GetKeyUp(int VK_CODE)
    {
        return GetInput().GetKeyUp(VK_CODE);
    }
    static bool GetKeyDown(int VK_CODE)
    {
        return GetInput().GetKeyDown(VK_CODE);
    }
    static Float2 GetMousePosition()
    {
        return GetInput().GetMousePosition();
    }
    static void SetMousePosition(float x, float y)
    {
        GetInput().SetMousePosition(x, y);
    }
    static void SetShowCursor(bool isShowCursor)
    {
        GetInput().SetShowCursor(isShowCursor);
    }
    static float GetTime()
    {
        return GetTimer().GetTime();
    }
    static float GetDeltaTime()
    {
        return GetTimer().GetDeltaTime();
    }
    static int GetFrameRate()
    {
        return GetTimer().GetFrameRate();
    }

private:
	static Window& GetWindow()
	{
		static std::unique_ptr<Window> window(new Window());
		return *window.get();
	}
	static Graphics& GetGraphics()
	{
		static std::unique_ptr<Graphics> graphics(new Graphics());
		return *graphics.get();
	}
    static Input& GetInput()
    {
        static std::unique_ptr<Input> input(new Input());
        return *input.get();
    }
    static Timer& GetTimer()
    {
        static std::unique_ptr<Timer> timer(new Timer());
        return *timer.get();
    }
};

#include "Texture.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
