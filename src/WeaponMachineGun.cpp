#include "WeaponMachineGun.h" 
#include "Entity.h"

CWeaponMachineGun::CWeaponMachineGun(){
    fire_rate=140;
    last_fire=0;
    base_damage=5;
    bonus_rand_damage=10;
    current_overheat=0.f;
    fired_this_tick=false;
}

CWeaponMachineGun::~CWeaponMachineGun(){

}

void CWeaponMachineGun::Fire(vec2_t &pos, float angle, bool plane_left){
    Uint32 curtime=SDL_GetTicks();
    fired_this_tick=true;
    float current_fire_rate=fire_rate+(current_overheat/max_overheat_time)*overheat_fire_rate_loss;
    if(curtime>last_fire+current_fire_rate){
        last_fire=curtime;   
        int recoil=rand()%10-5; 
        angle+=(float)recoil;
        angle*=M_PI/180.f;
        vbullets.push_back(SBullet(pos,angle)); 
        sound_manager->PlaySound(sound_effects_ids::MACHINE_GUN_SHOT);
    }
}

void CWeaponMachineGun::Simulate(float delta){

    if(fired_this_tick){
        current_overheat+=delta;
    }else{
        current_overheat-=delta*4.f;
    }

    if(current_overheat<0.f)
        current_overheat=0.f;
    if(current_overheat>max_overheat_time)
        current_overheat=max_overheat_time;
 
    for(std::vector<SBullet>::iterator it = vbullets.begin(); it != vbullets.end(); ){
        vec2_t vmove=vec2_t((delta*velocity)*cos(it->angle),(delta*velocity)*sin(it->angle));
        it->pos+=vmove;
        if (it->pos.x<-2||
            it->pos.x>SCREEN_WIDTH+2||
            it->pos.y<-2||
            it->pos.y>SCREEN_HEIGHT+2) {
            it = vbullets.erase(it);
        } else {
            ++it;
        }
    }

    fired_this_tick=false; 
    
}

void CWeaponMachineGun::Draw(vec2_t &plane_pos){
    SDL_SetRenderDrawColor(render->GetRenderer(),0,0,0,255);
    SDL_Rect bullet_rect;
    for(auto &bullet : vbullets){ 
        bullet_rect.x=(int)bullet.pos.x-2;
        bullet_rect.y=(int)bullet.pos.y-2;
        bullet_rect.w=4;
        bullet_rect.h=4;   
        SDL_RenderFillRect(render->GetRenderer(),&bullet_rect); 
    } 


    if(current_overheat!=0.f){
        SetOverHeatColor();
        float overheat_bar_width=30.f; 
        overheat_bar_width-=overheat_bar_width*(current_overheat/max_overheat_time);
        if(overheat_bar_width<1)
            overheat_bar_width=1;
        SDL_Rect bar_rect;
        bar_rect.x=(int)plane_pos.x-14;
        bar_rect.y=(int)plane_pos.y-20;
        bar_rect.h=3;
        bar_rect.w=(int)overheat_bar_width;
        SDL_RenderFillRect(render->GetRenderer(),&bar_rect);
    }

}

void CWeaponMachineGun::SetOverHeatColor(){
    const float max_color=220.f;
    float overheat=(current_overheat/max_overheat_time);
    if(overheat<0.5f){
        float p=overheat/0.5f;
        float r=max_color*p;
        SDL_SetRenderDrawColor(render->GetRenderer(),(int)r,(int)max_color,0,255);
    }else{
        float p=(overheat-0.5f)/0.5f;
        float g=max_color-max_color*p;
        SDL_SetRenderDrawColor(render->GetRenderer(),(int)max_color,(int)g,0,255); 
    }
}

bool CWeaponMachineGun::CheckCollision(vec2_t &plane_pos){
    float dist=PLANE_RADIUS+2; 
    for(std::vector<SBullet>::iterator it = vbullets.begin(); it != vbullets.end(); ){ 
        if (plane_pos.Distance(it->pos)<dist) {
            it = vbullets.erase(it);
            return true;
        } else {
            ++it;
        }
    } 
    return false;
}

void CWeaponMachineGun::Reset(){
    vbullets.clear();
    last_fire=0;
}
