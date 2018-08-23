#include "WeaponRocketLauncher.h" 
#include "Entity.h"

CWeaponRocketLauncher::CWeaponRocketLauncher(){
    fire_rate=2000;
    last_fire=0;
    base_damage=100;
    bonus_rand_damage=0; 
    active=false;
}

CWeaponRocketLauncher::~CWeaponRocketLauncher(){

}

void CWeaponRocketLauncher::Fire(vec2_t &pos, float angle, bool plane_left){  
    if(active)
        return;
    Uint32 curtime=SDL_GetTicks(); 
    float current_fire_rate=fire_rate;
    if(curtime>last_fire+current_fire_rate){
        last_fire=curtime;   
        int recoil=rand()%4-2; 
        fall_angle=angle+(plane_left?90.f:-90.f);
        angle+=(float)recoil;
        angle*=M_PI/180.f;
        fall_angle*=M_PI/180.f;
        active=true; 
        this->pos=pos;
        this->angle=angle;
        fall_velocity=max_fall_velocity;
        velocity=start_velocity;
        sound_manager->PlaySound(sound_effects_ids::ROCKET_SHOT);
    }
}

void CWeaponRocketLauncher::Simulate(float delta){
    if(!active)
        return; 

        delta/=1000;

    if(velocity<max_velocity)
        velocity+=acceleration*delta;

    if(fall_velocity>0)
        fall_velocity-=acceleration*delta;
    if(fall_velocity<0)
       fall_velocity=0;

    vec2_t vmove;
    vmove.x = velocity*cos(angle)*delta;
    vmove.y = velocity*sin(angle)*delta;

    vmove.x += fall_velocity*cos(fall_angle)*delta;
    vmove.y += fall_velocity*sin(fall_angle)*delta;

    pos+=vmove;

    if(pos.x+5<0||
       pos.x-5>SCREEN_WIDTH||
       pos.y+5<0||
       pos.y-5>SCREEN_HEIGHT){
       active=false;
    }
    
}

void CWeaponRocketLauncher::Draw(vec2_t &plane_pos){ 
    if(!active)
        return;
    vec2_t draw_pos=vec2_t(pos.x-9,pos.y-9);
    render->DrawTexture(texture_manager->GetTexture(BOMB), draw_pos, angle*180.f/M_PI); 
}
 
bool CWeaponRocketLauncher::CheckCollision(vec2_t &plane_pos){ 
    float dist=PLANE_RADIUS+9; 
    if (active&&plane_pos.Distance(pos)<dist) {
            active=false;
            return true; 
    }
    return false;
}

void CWeaponRocketLauncher::Reset(){
    active=false;
    last_fire=0;
}
