#include "Window.h"
#include "Graphics.h"
#include "Shader.h"
#include "Camera.h"

void Initialize()
{
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    Window::Initialize();
    Graphics::Initialize();
}

bool Refresh()
{
    Graphics::Update();
    return Window::Update();
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Initialize();

    auto camera = std::make_unique<Camera>();

    while (Refresh())
    {
        camera->Start();
        // ここで描画
        camera->Stop();
    }

    return 0;
}
