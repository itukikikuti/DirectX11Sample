#include "App.hpp"

int MAIN()
{
    Texture texture(L"../assets/texture.png");

    Mesh mesh;
    mesh.GetMaterial().SetTexture(0, &texture);

    Camera camera;
    camera.position = Float3(0.0f, 1.0f, -3.0f);
    camera.angles.x = 10.0f;

    App::SetMousePosition(0.0f, 0.0f);

    while (App::Refresh())
    {
        camera.angles += Float3(App::GetMousePosition().y, App::GetMousePosition().x, 0.0f) * 0.1f;
        camera.Update();

        App::SetMousePosition(0.0f, 0.0f);

        if (App::GetKey(VK_SPACE))
            Sleep(100);

        mesh.angles.y += App::GetDeltaTime();

        mesh.position.z = -1.0f;
        mesh.Draw();

        mesh.position.z = 1.0f;
        mesh.Draw();
    }

    return 0;
}
