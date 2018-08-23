#include "Game.h"

CTextureManager *texture_manager;
CSoundManager *sound_manager;
CFontManager *font_manager;
CKeyManager *key_manager;
CGameState* game_state; 
CGameRender *render;
Landscape * maps[NUMBER_OF_MAPS];
Landscape * active_map;
CGUI *gui;
CEntity * entities[PLANE_MAX];

int main(void){ 
    srand(time(NULL));

    CGame game;

    while(!game.Quit())
        game.Run();  

    SDL_Quit();
    romfsExit();
    return 0;
}


 
