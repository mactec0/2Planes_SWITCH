#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Render.h"
#include "TextureManager.h"
 
#define NUMBER_OF_MAPS 2

class Landscape{ 
public:
	Landscape(){}
	virtual ~Landscape(){}

	virtual void DrawBackground()=0;
	virtual void DrawForeground()=0;
	virtual void Control(float delta)=0;
 
};

extern  Landscape * active_map;
extern  Landscape * maps[NUMBER_OF_MAPS];
