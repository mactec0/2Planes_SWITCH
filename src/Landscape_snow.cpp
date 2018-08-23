#include "Landscape_snow.h"
 
 
LandscapeSnow::LandscapeSnow(){
    SDL_Surface *img_surface=texture_manager->GetSurface(BACKGROUND_1);
    background_raw=img_surface->pixels;
    snow_particles_timer.SetAlarm(40);
}

LandscapeSnow::~LandscapeSnow(){
	
}

void LandscapeSnow::DrawBackground(){ 
    render->DrawRawFast(background_raw);
}


void LandscapeSnow::DrawForeground(){

    SDL_SetRenderDrawColor(render->GetRenderer(),255,255,255,255);
    SDL_Rect part_pos;

    for(auto & particle : vParticles){
        part_pos.x=(int)particle.Pos.x;
        part_pos.y=(int)particle.Pos.y;
        part_pos.w=particle.size;
        part_pos.h=particle.size;   
        SDL_RenderFillRect(render->GetRenderer(),&part_pos); 
    }
}

void LandscapeSnow::Control(float delta){  
    if(snow_particles_timer.Alarm()){
	    if(vParticles.size()<MAX_PARTICLES){
            snow_particles_timer.Start(); 
	        SnowParticle new_particle;
	        new_particle.size=(std::rand()%3)+1;
	        new_particle.velocity=(std::rand()%90)+40;
	        new_particle.Pos.x=(std::rand()%1280)+0;
	        new_particle.Pos.y=-4;
	        new_particle.size=(std::rand()%3)+2;
	        vParticles.push_back(new_particle);
	    } 
    } 

    for(auto & particle : vParticles)
        particle.Pos.y+=delta*particle.velocity;
    

	vParticles.erase(std::remove_if(vParticles.begin(), vParticles.end(),
                     [](const SnowParticle& particle)
                       {
                       	if(particle.Pos.y>665){ 
                      		return true;
                       	}return false; 
                       }), vParticles.end()); 
    
      
} 

