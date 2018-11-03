// Window.cpp
#include "Window.hpp"

const wchar_t* Window::name = L"GameLib";
HWND Window::handle;

void Window::Initialize()
{
    HINSTANCE instance = GetModuleHandleW(nullptr);

    WNDCLASSW windowClass = {};
    windowClass.lpfnWndProc = ProceedMessage;
    windowClass.hInstance = instance;
    windowClass.hCursor = (HCURSOR)LoadImageW(nullptr, MAKEINTRESOURCEW(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED);
    windowClass.lpszClassName = name;
    RegisterClassW(&windowClass);

    handle = CreateWindowW(name, name, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, nullptr, nullptr, instance, nullptr);

    SetSize(640, 480);

    ShowWindow(handle, SW_SHOWNORMAL);
}

HWND Window::GetHandle()
{
    return handle;
}

DirectX::XMINT2 Window::GetSize()
{
    RECT clientRect = {};
    GetClientRect(handle, &clientRect);

    return DirectX::XMINT2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
}

void Window::SetSize(int width, int height)
{
    RECT windowRect = {};
    RECT clientRect = {};
    GetWindowRect(handle, &windowRect);
    GetClientRect(handle, &clientRect);

    int w = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + width;
    int h = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + height;
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    SetWindowPos(handle, nullptr, x, y, w, h, SWP_FRAMECHANGED);
}

bool Window::Update()
{
    MSG message = {};

    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
    {
        if (message.message == WM_QUIT)
            return false;

        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    return true;
}

LRESULT CALLBACK Window::ProceedMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY)
        PostQuitMessage(0);

    return DefWindowProcW(window, message, wParam, lParam);
}
