#include "App.hpp"

int MAIN()
{
    Texture texture(L"../assets/texture.png");

	Mesh mesh;
    mesh.GetMaterial().SetTexture(0, &texture);

    Camera camera;
    camera.position = Float3(0.0f, 1.0f, -3.0f);
    camera.angles.x = 10.0f;

	while (App::Refresh())
	{
        camera.Update();

        mesh.angles.y += 0.01f;

        mesh.position.z = -1.0f;
		mesh.Draw();

        mesh.position.z = 1.0f;
        mesh.Draw();
	}

	return 0;
}
