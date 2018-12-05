#include "Window.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"

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

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    Initialize();

    auto camera = std::make_unique<Camera>();
    camera->color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

    while (Refresh())
    {
        camera->Start();
        // ここで描画
        camera->Stop();
    }

    return 0;
}
