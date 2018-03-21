#include "App.hpp"

int MAIN()
{
	Mesh mesh;

	while (App::Refresh())
	{
        mesh.angle += 0.01f;
		mesh.Draw();
	}

	return 0;
}
