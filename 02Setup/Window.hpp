// Window.hpp
#pragma once
#define OEMRESOURCE
#include <Windows.h>
#include <DirectXMath.h>

class Window
{
public:
    static void Initialize();
    static HWND GetHandle();
    static DirectX::XMINT2 GetSize();
    static void SetSize(int width, int height);
    static bool Update();

private:
    static const wchar_t* name;
    static HWND handle;

    static LRESULT CALLBACK ProceedMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
};
