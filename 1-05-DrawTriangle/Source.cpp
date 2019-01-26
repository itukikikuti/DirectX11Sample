#include "Library.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    InitializeLibrary();

    auto camera = std::make_unique<Camera>();
    auto mesh = Mesh::CreateTriangle();

    while (UpdateLibrary())
    {
        camera->Start();
        mesh->Draw();
        camera->Stop();
    }

    return 0;
}
