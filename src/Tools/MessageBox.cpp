#include "MessageBox.h"
#include <SDL2/SDL_messagebox.h>

void	ShowMessageBox(const char* const title, const char* const message)
{
	SDL_ShowSimpleMessageBox(0, title, message, nullptr);
}
