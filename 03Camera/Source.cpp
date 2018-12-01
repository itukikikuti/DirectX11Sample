// Source.cpp
#include "Window.hpp"
#include "Graphics.hpp"

#include "Camera.hpp"

void Initialize()
{
    Window::Initialize();
    Graphics::Initialize();
}

bool Refresh()
{
    Graphics::Update();
    return Window::Update();
}

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    Initialize();

    Camera camera;

    while (Refresh())
    {
        camera.Start();
        // ここで描画
        camera.Stop();
    }

    return 0;
}
