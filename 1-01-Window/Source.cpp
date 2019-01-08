#include "Window.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Window::Initialize();

    while (Window::Update())
    {
        // ここにゲームの処理を書いていく
    }

    return 0;
}
