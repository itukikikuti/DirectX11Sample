#include "App.hpp"

int Main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Mesh mesh;

	while (App::Refresh())
	{
		mesh.Draw();
	}

	return 0;
}
