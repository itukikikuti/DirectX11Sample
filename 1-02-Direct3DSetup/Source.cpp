#include "Library.h"

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    InitializeLibrary();

    while (UpdateLibrary())
    {
        // ここにゲームの処理を書いていく
    }

    return 0;
}
