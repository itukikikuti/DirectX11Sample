#include "Library.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    InitializeLibrary();

    auto camera = std::make_unique<Camera>();
    camera->color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

    while (UpdateLibrary())
    {
        camera->Start();
        // ここで描画
        camera->Stop();
    }

    return 0;
}
