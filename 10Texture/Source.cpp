#include "App.hpp"

int Main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Texture texture(L"texture.png");
	Mesh mesh;

	mesh.material.SetTexture(0, &texture);

	while (App::Refresh())
	{
		mesh.Draw();
	}

	return 0;
}
