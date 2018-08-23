#pragma once 
#include <switch.h>  
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Landscape_green.h"
#include "Landscape_snow.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "KeyManager.h" 
#include "GameState.h"
#include "Render.h"
#include "Entity.h"
#include "Timer.h"
#include "GUI.h"


class CGame{  
    Timer after_death_timer; 
    Uint32 last_time;
    int winner;

public:
    CGame();
    ~CGame();
    void Run();
    bool Quit();

};