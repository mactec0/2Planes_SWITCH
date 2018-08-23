#include "Landscape_green.h"
 

LandscapeGreen::LandscapeGreen(){
	SDL_Surface *img_surface=texture_manager->GetSurface(BACKGROUND_2); 
	background_raw=img_surface->pixels; 
}

LandscapeGreen::~LandscapeGreen(){
	
}

void LandscapeGreen::DrawBackground(){ 
    render->DrawRawFast(background_raw);
}

void LandscapeGreen::DrawForeground(){
}

void LandscapeGreen::Control(float delta){  
    
} 

