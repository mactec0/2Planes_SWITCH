#include "WeaponLaser.h" 
#include "Entity.h"

CWeaponLaser::CWeaponLaser(){
    fire_rate=1200;
    last_fire=0;
    last_attack=0;
    base_damage=3;
    bonus_rand_damage=1;  
    this->Reset();
}

CWeaponLaser::~CWeaponLaser(){

}

void CWeaponLaser::Fire(vec2_t &pos, float angle, bool plane_left){
    Uint32 curtime=SDL_GetTicks(); 

    this->angle=angle*M_PI/180.f;
    this->pos=pos;

    if(to_fire){ 
        to_fire=false;  
    }

    if(can_fire_again&&curtime>last_fire+fire_rate){
        active = true;
        can_fire_again=false;
        last_fire=curtime;
        sound_channel=sound_manager->PlaySound(sound_effects_ids::LASER_BEAM_SHOT);
    }
}

void CWeaponLaser::Simulate(float delta){ 
    if(to_fire){
        if(!can_fire_again)
        sound_manager->StopSound(sound_channel);
        active=false;
        can_fire_again=true;
    }

     

    to_fire=true; 

    if(last_fire+480<SDL_GetTicks()){ 
        active=false; 
    }
}

void CWeaponLaser::Draw(vec2_t &plane_pos){
    if(!active)
        return;

    SDL_SetRenderDrawColor(render->GetRenderer(),125,0,250,255);
    
    vec2_t start_pos=plane_pos;

    vec2_t next_pos = start_pos;
    next_pos.x+=cos(angle)*1500.f;
    next_pos.y+=sin(angle)*1500.f;
    SDL_RenderDrawLine(render->GetRenderer(), (int)start_pos.x, (int)start_pos.y, (int)next_pos.x, (int)next_pos.y);

    SDL_SetRenderDrawColor(render->GetRenderer(),220,0,200,255);
    start_pos+=vec2_t(1.f,1.f);
    next_pos+=vec2_t(1.f,1.f);
    SDL_RenderDrawLine(render->GetRenderer(), (int)start_pos.x, (int)start_pos.y, (int)next_pos.x, (int)next_pos.y); 
    
} 

bool CWeaponLaser::CheckCollision(vec2_t &plane_pos){ 
    if(!active)
        return false;

    float collision_radius=PLANE_RADIUS+2; 
    float distance=plane_pos.Distance(pos);
    vec2_t next_pos = pos;
    next_pos.x+=cos(angle)*distance;
    next_pos.y+=sin(angle)*distance;
    
    Uint32 curtime=SDL_GetTicks(); 
    if(last_attack+30<curtime)
    if (plane_pos.Distance(next_pos)<collision_radius) { 
        last_attack=curtime;
        return true;
    } 
    return false;
}

void CWeaponLaser::Reset(){
    active=false;
    to_fire=false;
    can_fire_again=true;
}
