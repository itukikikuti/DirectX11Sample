#include "Library.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	InitializeLibrary();

    auto camera = std::make_unique<Camera>();
    camera->position = DirectX::XMFLOAT3(0.0f, 2.0f, -2.0f);
    camera->rotation = DirectX::XMFLOAT3(45.0f, 0.0f, 0.0f);

    auto texture = Texture::CreateFromFile(L"Texture.jpg");

    auto mesh = Mesh::CreateCube(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    mesh->texture = *texture;

    while (UpdateLibrary())
    {
        camera->Start();

        mesh->rotation.y += 1.0f;
        mesh->Draw();

        camera->Stop();
    }

    return 0;
}
