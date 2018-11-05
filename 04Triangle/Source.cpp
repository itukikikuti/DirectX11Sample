// Source.cpp
#include "Window.hpp"
#include "Graphics.hpp"

#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"

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
    Mesh mesh;

    while (Refresh())
    {
        camera.Start();
        
        mesh.Draw();

        camera.Stop();
    }

    return 0;
}
