#include <SDL2/SDL.h>
#include "Tools/debug.h"
#include "Tools/Log.h"
#include "CGame.h"

int main(int argc, char *argv[])
{
#if defined(_MSC_VER) && defined(_DEBUG) && defined(DD_TRACK_ALLOCATIONS)
	int	flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(flag);
#endif
	
	CleanLogFile();

	CGame	*Game = DD_NEW CGame;

	if (Game->Init() == false)
	{
		Log("Game Init failed");
	}
	else
	{
		if (Game->RunGameLoop() == false)
		{
			Log("Game loop closed");
		}
	}

	Game->Clean();

	DD_DELETE(Game);
	
#if defined(_WIN32) && defined(_DEBUG) && defined(DD_TRACK_ALLOCATIONS)
	_CrtDumpMemoryLeaks();
#endif

	return	0;
}