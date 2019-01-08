#include "Window.h"

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    Window::Initialize();

    while (Window::Update())
    {
        // ここにゲームの処理を書いていく
    }

    return 0;
}
