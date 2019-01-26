#include "Library.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    InitializeLibrary();

    auto camera = std::make_unique<Camera>();
    camera->position = DirectX::XMFLOAT3(0.0f, 2.0f, -2.0f);
    camera->rotation = DirectX::XMFLOAT3(45.0f, 0.0f, 0.0f);

    auto mesh = Mesh::CreateTriangle();

    float t = 0.0f;

    while (UpdateLibrary())
    {
        camera->Start();

        t += 1.0f;
        mesh->position.x = cosf(t * 0.01f);
        mesh->rotation.y = t * 1.0f;
        mesh->scale.x = 1.0f + cosf(t * 0.1f) * 0.5f;
        mesh->Draw();

        camera->Stop();
    }

    return 0;
}
