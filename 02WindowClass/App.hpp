#pragma once

#define OEMRESOURCE
#include <memory>
#include <vector>
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define Main() APIENTRY wWinMain(HINSTANCE hinstance, HINSTANCE, LPWSTR, int)
#define PUBLIC public:
#define PRIVATE private:
#define PROTECTED protected:

class App final
{
	PUBLIC static constexpr wchar_t* const NAME = L"02WindowClass";

#include "Window.hpp"

	PUBLIC App() = delete;
	PUBLIC static bool Refresh()
	{
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
	PRIVATE static Window& GetWindow()
	{
		static std::unique_ptr<Window> window(new Window());
		return *window.get();
	}
};
