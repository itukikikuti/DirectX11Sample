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

#define Main() APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
#define PUBLIC public:
#define PRIVATE private:
#define PROTECTED protected:

#include "Math.hpp"

class App final
{
	PUBLIC static constexpr wchar_t* const NAME = L"09Shading";

#include "Window.hpp"
#include "Graphics.hpp"

	PUBLIC App() = delete;
	PUBLIC static bool Refresh()
	{
		GetGraphics().Update();
		return GetWindow().Update();
	}
	PUBLIC static HWND GetWindowHandle()
	{
		return GetWindow().GetHandle();
	}
	PUBLIC static DirectX::XMINT2 GetWindowSize()
	{
		return GetWindow().GetSize();
	}
	PUBLIC static void SetWindowSize(int width, int height)
	{
		return GetWindow().SetSize(width, height);
	}
	PUBLIC static ID3D11Device& GetGraphicsDevice()
	{
		return GetGraphics().GetDevice();
	}
	PUBLIC static ID3D11DeviceContext& GetGraphicsContext()
	{
		return GetGraphics().GetContext();
	}
	PRIVATE static Window& GetWindow()
	{
		static std::unique_ptr<Window> window(new Window());
		return *window.get();
	}
	PRIVATE static Graphics& GetGraphics()
	{
		static std::unique_ptr<Graphics> graphics(new Graphics());
		return *graphics.get();
	}
};

#include "Material.hpp"
#include "Mesh.hpp"
