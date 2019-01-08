#include "Window.h"
#include "Graphics.h"
#include "CBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

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
    camera->position = DirectX::XMFLOAT3(0.0f, 2.0f, -2.0f);
    camera->rotation = DirectX::XMFLOAT3(45.0f, 0.0f, 0.0f);

    auto texture = Texture::CreateFromFile(L"Texture.jpg");

    auto mesh = Mesh::CreateCube(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    mesh->texture = *texture;

    float angle = 0.0f;

    while (Refresh())
    {
        camera->Start();

        angle += 1.0f;

        mesh->rotation.y = angle;
        mesh->Draw();

        camera->Stop();
    }

    return 0;
}
