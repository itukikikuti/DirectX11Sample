#pragma once

#define OEMRESOURCE
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define Main() APIENTRY wWinMain(HINSTANCE hinstance, HINSTANCE, LPWSTR, int)
#define PUBLIC public:
#define PRIVATE private:
#define PROTECTED protected:

class App
{
	PUBLIC static constexpr wchar_t* const NAME = L"Test";

#include "Window.hpp"

	PUBLIC App() = delete;
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
