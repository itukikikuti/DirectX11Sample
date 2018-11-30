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

    float temp = 0.0f;

    while (Refresh())
    {
        camera.Start();
        
        temp += 0.1f;

        mesh.position = DirectX::XMFLOAT3(cosf(temp), sinf(temp), 0.0f);
        mesh.rotation.y = temp * 10.0f;
        mesh.Draw();

        camera.Stop();
    }

    return 0;
}
