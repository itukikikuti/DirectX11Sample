// Source.cpp
#include "Window.hpp"

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    Window::Initialize();

    while (Window::Update())
    {
        // ‚±‚±‚ÉƒQ[ƒ€‚Ìˆ—‚ğ‘‚¢‚Ä‚¢‚­
    }

    return 0;
}
