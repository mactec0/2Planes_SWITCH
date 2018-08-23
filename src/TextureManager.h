#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Render.h"

#include <iostream>
#include <string>


enum textures_ids{
	PLANE_1,
	PLANE_2, 

	MENU_BACKGROUND,

	BACKGROUND_1,
	BACKGROUND_2,

	BOMB,

	CRASH_0,
	CRASH_1,
	CRASH_2,
	CRASH_3,
	CRASH_4,
	CRASH_5,
	CRASH_6,
	CRASH_7,
	CRASH_8,
	CRASH_9,
	CRASH_10,
	CRASH_11,
	CRASH_12,
	CRASH_13,

	SMOKE_0,
	SMOKE_1,
	SMOKE_2,
	SMOKE_3,
	SMOKE_4,
	SMOKE_5,

	HEART,

	SETTINGS_0,
	SETTINGS_1,

	TEX_MAX

};

class CTextureManager{
	SDL_Surface *surfaces[TEX_MAX];
	SDL_Texture *textures[TEX_MAX];
	bool initialized;

	SDL_Surface* LoadSurface(const char *path); 
	SDL_Texture* LoadTexture(const char *path, int ID);

public:

	CTextureManager();
	~CTextureManager();
	bool IsInitialized();

	SDL_Surface* GetSurface(int ID);
	SDL_Texture* GetTexture(int ID);
};

extern CTextureManager *texture_manager;