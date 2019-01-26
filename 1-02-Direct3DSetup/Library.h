#pragma once

#define OEMRESOURCE

#include <vector>

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Window.h"
#include "Graphics.h"

inline void InitializeLibrary()
{
    static bool isInitialized = false;

    if (!isInitialized)
    {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        Window::Initialize();
        Graphics::Initialize();

        isInitialized = true;
    }
}

inline bool UpdateLibrary()
{
    Graphics::Update();
    return Window::Update();
}

#pragma comment(lib, "d3d11.lib")
