// Source.cpp
#include "Window.hpp"
#include "Graphics.hpp"

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

    while (Refresh())
    {
        // �����ɃQ�[���̏����������Ă���
    }

    return 0;
}
