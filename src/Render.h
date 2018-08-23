#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "vec2.h"
#include "FontManager.h" 

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class CGameRender{
    bool initialized;
    SDL_Window *window;
    SDL_Renderer* renderer;

    float frame_cap;
    unsigned int begin_frame_time, fps_timer;
    int frame_count;
    int fps;
    bool show_fps;

    SDL_Surface* fps_surface;
    SDL_Texture* fps_tex;

    void CreateFPSTexture();

public:
    CGameRender();
    ~CGameRender();

    void DrawTexture(SDL_Texture* texture, vec2_t &vpos);
    void DrawTexture(SDL_Texture* texture, vec2_t &vpos, float angle);
    void DrawPlaneTexture(SDL_Texture* texture, vec2_t &vpos, float angle);

    void SwapBuffers();

    void BeginFrame();

    bool IsInitialized();

    SDL_Renderer* GetRenderer();

    SDL_Window* GetWindow();

    void SetFramerateLimit(int limit); 

    void Printf(vec2_t pos, bool center, SDL_Color col, int Font, char* _Input, ...);

    void DrawRawFast(void *raw); 

};

extern CGameRender *render;