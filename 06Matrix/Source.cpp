// Source.cpp
#include "Window.hpp"
#include "Graphics.hpp"

#include "CBuffer.hpp"
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
    camera.position = DirectX::XMFLOAT3(0.0f, 1.0f, -5.0f);
    camera.rotation = DirectX::XMFLOAT3(10.0f, 0.0f, 0.0f);

    Mesh mesh;

    while (Refresh())
    {
        camera.Start();
        
        mesh.rotation.y += 1.0f;
        mesh.Draw();

        camera.Stop();
    }

    return 0;
}
