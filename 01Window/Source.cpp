// Source.cpp
#include "Window.hpp"

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    while (Window::Update())
    {
        // ここにゲームの処理を書いていく
    }

    return 0;
}
