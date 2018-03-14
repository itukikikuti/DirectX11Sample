#include "App.hpp"

int Main()
{
	Mesh mesh;

	while (App::Refresh())
	{
		mesh.Draw();
	}

	return 0;
}
