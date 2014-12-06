#pragma once

#include <map>
#include <vector>

#include "../Input/InputData.h"

enum class RenderAPI;
typedef struct SDL_RWops SDL_RWops;
struct _texture;
struct _textureSFML;
struct _rect;

namespace DD
{
	namespace IO
	{
		// sets up globals and api
		// api will be set as SDL if there's no config file or some other error happens
		bool	LoadConfigFile(RenderAPI &api);

		// load key configuration
		// if there's no key configuration file returns false (then use default keys provided by CInput)
		bool	LoadKeysConfiguration(std::map<std::string, _mappedKey> &mappedKey_, std::map<std::string,_mappedMouse> &mappedMouse_,
			std::map<std::string, _mappedLeftRight> &mappedLeftRight_, std::map<std::string, _mappedUpDown> &mappedUpDown_, std::map<std::string, KeyboardEvents> &eventsKey,
			std::map<std::string, MouseEvents> &eventsMouse, std::map<std::string, LeftRight> &eventsLeftRight, std::map<std::string, UpDown> &eventsUpDown);
	
		// an element will be updated with true if it's traversable and false if it's not
		// returns false on error and writes to log
		bool	LoadPassableFloor(bool array_[]);

		bool	LoadCollisionRects(std::vector<_rect> &pRects, std::string datName);

		// Unicode friendly !
		// loads all file names from a folder into fileNames
		// path_ -> path inside the project
		// returns false on error and writes to log
		bool	LoadFileNames(std::vector<std::string> &fileNames,const char * const path_);

		// used for loading files
		// writes to log if file load fails
		// check return value for nullptr
		SDL_RWops*	LoadFile(const char* fileName);

		void		Clean();
	};
};
