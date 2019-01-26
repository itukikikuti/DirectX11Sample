#pragma once
#include "Library.h"

class Window
{
public:
    static void Initialize();
    static HWND GetHandle();
    static DirectX::XMUINT2 GetSize();
    static void SetSize(int width, int height);
    static bool Update();

private:
    static HWND handle;

    static LRESULT CALLBACK ProceedMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
};
