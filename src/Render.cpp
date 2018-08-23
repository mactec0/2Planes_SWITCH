#include "Render.h"


CGameRender::CGameRender(){
	initialized=true;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL_Init: %s\n", SDL_GetError());
        initialized = false;
        return;
    }

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    frame_cap=0; 

    frame_count=0;
    fps_timer=0;
    fps=0;
    show_fps=false;

    fps_surface=NULL;
    fps_tex=NULL;

}

CGameRender::~CGameRender(){


}

void CGameRender::SetFramerateLimit(int limit){
    frame_cap=limit;
}


void CGameRender::BeginFrame(){
    begin_frame_time=SDL_GetTicks();
}

void CGameRender::SwapBuffers(){ 
    static bool first_run = true;
    if(first_run){
        first_run=false;
        CreateFPSTexture();
    }
 
    if(show_fps){
        vec2_t fps_pos=vec2_t(10.f,10.f);
        this->DrawTexture(fps_tex, fps_pos); 
    }


    SDL_RenderPresent(renderer);

    frame_count++;
    int current_fps_timer = SDL_GetTicks();
    if (current_fps_timer > fps_timer + 1000) { 
        fps_timer=current_fps_timer;
        fps=frame_count;
        frame_count=0;
        CreateFPSTexture();
    }
 
    float frame_diff=current_fps_timer-begin_frame_time;
    if(frame_cap&&(frame_diff<1000/frame_cap)){
        SDL_Delay((1000/frame_cap)-frame_diff);
    }
}

bool CGameRender::IsInitialized(){
	return this->initialized;
}

void CGameRender::DrawTexture(SDL_Texture* texture, vec2_t &vpos){
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect rect;
    rect.h = h;
    rect.w = w;
    rect.x = (int)vpos.x;
    rect.y = (int)vpos.y; 
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}


void CGameRender::DrawTexture(SDL_Texture* texture, vec2_t &vpos, float angle){ 
    SDL_Point sprite_center;
    int w, h;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect rect;
    rect.h = h;
    rect.w = w;
    rect.x = (int)vpos.x;
    rect.y = (int)vpos.y; 
    sprite_center.x = rect.w/2; sprite_center.y=rect.h/2;
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &sprite_center,flip);
}

void CGameRender::DrawPlaneTexture(SDL_Texture* texture, vec2_t &vpos, float angle){
	SDL_Point sprite_center;
	SDL_Rect rect;
    rect.x=(int)vpos.x-21;
    rect.y=(int)vpos.y-21;
    rect.w=42;
    rect.h=42;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    sprite_center.x = 21; sprite_center.y=21;
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &sprite_center,flip);

}
 
SDL_Renderer* CGameRender::GetRenderer(){
    return renderer;
}

SDL_Window* CGameRender::GetWindow(){
    return window;
}



void CGameRender::CreateFPSTexture(){
    if(!show_fps)
        return;
    if(fps_surface)
    SDL_FreeSurface(fps_surface);
    if(fps_tex)
    SDL_DestroyTexture(fps_tex);

    char Buffer[256] = { 0x00 };

    sprintf (Buffer, "FPS: %d ", fps);


    SDL_Color White = {50, 240, 125};
    fps_surface = TTF_RenderText_Solid(font_manager->GetFont(0), Buffer, White); 
       

    if(!fps_surface)
        return;

    fps_tex = SDL_CreateTextureFromSurface(this->GetRenderer(), fps_surface);

    if(!fps_tex){
        SDL_FreeSurface(fps_surface);
        return; 
    }


}

void CGameRender::Printf(vec2_t pos, bool center, SDL_Color col, int Font, char* _Input, ...){
    char Buffer[1024] = { '\0' };
 
    va_list Args;

    va_start(Args, _Input);
    vsprintf(Buffer, _Input, Args);
    va_end(Args);

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font_manager->GetFont(Font), Buffer, col);
 
    if(!surfaceMessage)
        return;

    SDL_Texture* Message = SDL_CreateTextureFromSurface(this->GetRenderer(), surfaceMessage);

    if(!Message){
        SDL_FreeSurface(surfaceMessage);
        return; 
    }

    if(center){
        int w, h; 
        SDL_QueryTexture(Message, NULL, NULL, &w, &h);
        pos.x-=w/2;
        pos.y-=h/2;

    }

    this->DrawTexture(Message, pos); 

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}


void CGameRender::DrawRawFast(void *raw){
    SDL_Surface * screen = SDL_GetWindowSurface(window); 
    memcpy(screen->pixels, raw, 1280*720*4);
}

 


