#include "App.hpp"

int MAIN()
{
    Texture texture(L"../assets/texture.png");
	Mesh mesh;

    mesh.GetMaterial().SetTexture(0, &texture);

	while (App::Refresh())
	{
        mesh.angles.y += 0.01f;
		mesh.Draw();
	}

	return 0;
}
