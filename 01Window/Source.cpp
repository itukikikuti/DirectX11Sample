// Source.cpp
#include "Window.hpp"

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    Window::Initialize();

    while (Window::Update())
    {
        // �����ɃQ�[���̏����������Ă���
    }

    return 0;
}
