#include "Library.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    InitializeLibrary();

    auto camera = std::make_unique<Camera>();
    Shader& shader = Shader::GetDefault();

    while (UpdateLibrary())
    {
        camera->Start();
        shader.Attach();
        camera->Stop();
    }

    return 0;
}
