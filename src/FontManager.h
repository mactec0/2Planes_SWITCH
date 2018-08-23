#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Render.h"

enum fonts_ids{
	MENU_1,
	MENU_2,
	MENU_3,
	MENU_LOGO,
	FONTS_MAX

};

class CFontManager{
	TTF_Font *fonts[fonts_ids::FONTS_MAX]; 
	bool initialized; 

	TTF_Font* LoadFont(const char *path, int ID, int size); 

public:

	CFontManager();
	~CFontManager();
	bool IsInitialized();

	TTF_Font* GetFont(int ID);
};

extern CFontManager *font_manager;
