#include "IOFunctions.h"
#include <SDL2/SDL.h>

#ifndef __RPC_H__
#include <rpc.h>
#endif

#include "fileapi.h"
#include "handleapi.h"
#include "../Render/CSDLImage.h"
#include "../Render/CSFMLImage.h"
#include "../Logic/Physics/CCollisionData.h"
#include "../Tools/Functions.h"
#include "../Tools/StringWrapper.h"
#include "../Tools/Log.h"
#include "../Tools/debug.h"
#include "../Data/Globals.h"
#include "../Data/Const.h"
#include "../Data/Enums.h"

#if defined (DD_DEBUG)
static  const char* const  FULL_PATH = PROJECT_FOLDER_PATH;
#else
static  const char* const FULL_PATH = SDL_GetBasePath();
#endif

void	DD::IO::Clean()
{
#if !defined(DD_DEBUG)
	SDL_free((char*)FULL_PATH);
#endif
}

// returned string must be freed when it's no longer needed, otherwise it will leak
static wchar_t *ToLPCWSTR(const char* const charArray, const size_t size)
{
	wchar_t * str = DD_NEW wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, charArray, -1, str, static_cast<int>(size));
	return str;
}

static void ToLPCWSTR(LPWSTR des, const char* const src, const size_t size)
{
	MultiByteToWideChar(CP_UTF8, 0, src, -1, des, static_cast<int>(size));
}

static void ToMultiByteChar(char *des, LPCWSTR src, const size_t size)
{
	WideCharToMultiByte(CP_UTF8, 0, src, -1, des, static_cast<int>(size), nullptr, nullptr);
}

bool DD::IO::LoadConfigFile(RenderAPI &api)
{
	SDL_RWops *f = LoadFile(CONFIG_FILE_NAME);

	if (f == nullptr)
	{
		api = RenderAPI::SDL; // used so that we have a default value if there's no config file
		return	false;
	}

	char line[256] = "\0";

	f->read(f, line, sizeof(line), 1);

	char *context = nullptr;
	char *word = DD_Strtok(line, "\r\n", &context);
	std::string found = "";

	while (word != nullptr)
	{

		// we do not need to set a default value if LineParse returns false because for these globals a default value has already been set

		found = word;

		if (found.find("width") != std::string::npos)
		{
			if (DD::LineParse(word) == true)
			{
				g_windowWidth = atoi(word);
			}
		}
		else if (found.find("height") != std::string::npos)
		{
			if (DD::LineParse(word) == true)
			{
				g_windowHeight = atoi(word);
			}
		}
		else if (found.find("hud") != std::string::npos)
		{
			if (DD::LineParse(word) == true)
			{
				g_windowHUDHeight = atoi(word);
			}
		}
		else if (found.find("limit fps") != std::string::npos)
		{
			if (DD::LineParse(word) == true)
			{
				int val = atoi(word);

				if (val)
					CGlobal::SetLimitFPS(true);
				else
					CGlobal::SetLimitFPS(false);
			}
		}
		else if (found.find("limit cpu") != std::string::npos)
		{
			if (DD::LineParse(word) == true)
			{
				int val = atoi(word);

				if (val)
					CGlobal::SetLimitCPU(true);
				else
					CGlobal::SetLimitCPU(false);
			}
		}
		else if (found.find("api") != std::string::npos)
		{
			if (DD::LineParse(word) == true)
			{
				if (std::char_traits<char>::compare("SDL", word, 3) == 0)
				{
					api = RenderAPI::SDL;
				}
				else if (std::char_traits<char>::compare("SFML", word, 4) == 0)
				{
					api = RenderAPI::SFML;
				}
			}
		}

		word = DD_Strtok(nullptr, "\r\n", &context);
	}

	f->close(f);

	return	true;
}

bool	DD::IO::LoadKeysConfiguration(std::map<std::string, _mappedKey> &mappedKey_, std::map<std::string, _mappedMouse> &mappedMouse_,
	std::map<std::string, _mappedLeftRight> &mappedLeftRight_, std::map<std::string, _mappedUpDown> &mappedUpDown_, std::map<std::string, KeyboardEvents> &eventsKey,
	std::map<std::string, MouseEvents> &eventsMouse, std::map<std::string, LeftRight> &eventsLeftRight, std::map<std::string, UpDown> &eventsUpDown)
{
	SDL_RWops	*f = LoadFile(KEYS_FILE_NAME);

	if (f == nullptr)
		return	false;

	char line[1024] = "\0";

	f->read(f, line, sizeof(line), 1);

	char *context = nullptr;
	char *word = DD_Strtok(line, "\r\n", &context);

	std::string found = "";
	std::string afterStr = "";
	std::string beforeStr = "";

	char beforeCStr[32] = "\0";
	size_t size = sizeof(beforeCStr);

	enum class FoundInput { nothing, keyboard, mouse, leftRight, upDown };

	FoundInput foundInput{ FoundInput::nothing };

	while (word != nullptr)
	{
		found = word;

		if (found.find("keyboard") != std::string::npos)
		{
			foundInput = FoundInput::keyboard;
		}
		else if (found.find("upDown") != std::string::npos)
		{
			foundInput = FoundInput::upDown;
		}
		else if (found.find("leftRight") != std::string::npos)
		{
			foundInput = FoundInput::leftRight;
		}
		else if (found.find("mouse") != std::string::npos)
		{
			foundInput = FoundInput::mouse;
		}
		else
		{
			if (foundInput == FoundInput::keyboard)
			{
				if (DD::LineParse(word, beforeCStr, size) == true)
				{
					afterStr = word;
					beforeStr = beforeCStr;
					mappedKey_[beforeStr] = _mappedKey(afterStr, eventsKey[afterStr]);
				}
			}
			else if (foundInput == FoundInput::leftRight)
			{
				if (DD::LineParse(word, beforeCStr, size) == true)
				{
					afterStr = word;
					beforeStr = beforeCStr;
					mappedLeftRight_[beforeStr] = _mappedLeftRight(afterStr, eventsLeftRight[afterStr]);
				}
			}
			else if (foundInput == FoundInput::upDown)
			{
				if (DD::LineParse(word, beforeCStr, size) == true)
				{
					afterStr = word;
					beforeStr = beforeCStr;
					mappedUpDown_[beforeStr] = _mappedUpDown(afterStr, eventsUpDown[afterStr]);
				}
			}
			else if (foundInput == FoundInput::mouse)
			{
				if (DD::LineParse(word, beforeCStr, size) == true)
				{
					afterStr = word;
					beforeStr = beforeCStr;
					mappedMouse_[beforeStr] = _mappedMouse(afterStr, eventsMouse[afterStr]);
				}
			}
		}


		word = DD_Strtok(nullptr, "\r\n", &context);
	}

	f->close(f);

	return	true;
}

bool	DD::IO::LoadCollisionRects(std::vector<_rect> &pRects, std::string datName)
{
	std::string fullName{ "data/collision/" + datName };

	SDL_RWops *file = LoadFile(fullName.c_str());

	char line[32768] = "\0";

	file->read(file, line, sizeof(line), 1);

	char *context = nullptr;
	char *word = DD_Strtok(line, "\r\n", &context);
	std::string found = "";

	while (word != nullptr)
	{
		found = word;

		if (found.find("index") != std::string::npos)
		{
			pRects.push_back(_rect{});
		}
		else if (found.find("rect") != std::string::npos)
		{
			pRects.back().rect.push_back(_rectData{});
		}
		else if (found.find("x") != std::string::npos)
		{
			if (DD::LineParse(word))
			{
				pRects.back().rect.back().x = atoi(word);
			}
			else 
			{
				file->close(file);
				return	false;
			}
		}
		else if (found.find("y") != std::string::npos)
		{
			if (DD::LineParse(word))
			{
				pRects.back().rect.back().y = atoi(word);
			}
			else
			{
				file->close(file);
				return	false;
			}
		}
		else if (found.find("w") != std::string::npos)
		{
			if (DD::LineParse(word))
			{
				pRects.back().rect.back().w = atoi(word);
			}
			else
			{
				file->close(file);
				return	false;
			}
		}
		else if (found.find("h") != std::string::npos)
		{
			if (DD::LineParse(word))
			{
				pRects.back().rect.back().h = atoi(word);
			}
			else
			{
				file->close(file);
				return	false;
			}
		}
		else
		{
			file->close(file);
			return	false;
		}

		word = DD_Strtok(nullptr, "\r\n", &context);
	}

	file->close(file);

	return	true;
}

bool	DD::IO::LoadFileNames(std::vector<std::string>	&fileNames,const char * const path_)
{
	WIN32_FIND_DATAW file;
	HANDLE find = nullptr;

	char path[2048] = "\0";
	int size_ = sizeof(path);

	DD_Strcpy(path, size_, FULL_PATH);
	DD_Strcat(path, size_, path_);
	DD_Strcat(path, size_, "\\*.*");

	LPCWSTR pathUtf8 = ToLPCWSTR(path,2048);
	LPCWSTR strDot = ToLPCWSTR(".", 2);
	LPCWSTR str2Dot = ToLPCWSTR("..", 3);
	
	if ((find = FindFirstFileW(pathUtf8, &file)) == INVALID_HANDLE_VALUE)
	{
		Log("DD:IO::LoadMapFileNames failed,wrong path: ");
		Log(path);

		DD_DELETE_ARRAY(pathUtf8);
		DD_DELETE_ARRAY(strDot);
		DD_DELETE_ARRAY(str2Dot);

		return	false;
	}

	do
	{
		if (wcscmp(file.cFileName, strDot) != 0 && wcscmp(file.cFileName, str2Dot) != 0) // first two files
		{
			char fileName[256];
			ToMultiByteChar(fileName, file.cFileName, 256);
			fileNames.push_back(fileName);
		}
	} while (FindNextFileW(find, &file));

	FindClose(find);

	DD_DELETE_ARRAY(pathUtf8);
	DD_DELETE_ARRAY(strDot);
	DD_DELETE_ARRAY(str2Dot);

	return	true;
}

bool	DD::IO::LoadPassableFloor(bool array_[])
{

	SDL_RWops *f = LoadFile(FLOOR_PASSABLE_FILE_NAME);

	char line[15 * 1024] = "\0";

	f->read(f, line, sizeof(line), 1);

	char *context = nullptr;
	char *word = DD_Strtok(line, "\r\n", &context);

	int index = 0;

	while (word != nullptr)
	{
		if (index > IMG_FLOOR_FINISH)
		{
			Log("DD::IO::LoadPassableFloor(bool[]) failed, index larger than array size");
			return	false;
		}

		if (DD::LineParse(word) == true)
		{
			if (std::char_traits<char>::compare("p", word, 1) == 0)
			{
				array_[index] = false;
			}
			else if (std::char_traits<char>::compare("n", word, 1) == 0)
			{
				array_[index] = true;
			}
			else
			{
				Log("DD::IO::LoadPassableFloor(bool[]) failed, wrong value at: ", index);
				return	false;
			}
		}
		else
		{
			Log("DD::IO::LoadPassableFloor(bool[]) failed, Line parsing ( no = sign) failed at ", index);

			return	false;
		}

		index++;
		word = DD_Strtok(nullptr, "\r\n", &context);
	}

	f->close(f);

	return	true;
}

SDL_RWops* DD::IO::LoadFile(const char* fileName)
{
	SDL_RWops	*f = SDL_RWFromFile(fileName, "r");

	if (f == nullptr)
	{
		char t[64];
		DD_Strcpy(t, sizeof(t), fileName);
		DD_Strncat(t, sizeof(t), " failed to load", 15);

		Log(t);

		return	nullptr;
	}

	return	f;
}
