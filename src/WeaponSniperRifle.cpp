#include "WeaponSniperRifle.h" 
#include "Entity.h"

CWeaponSniperRifle::CWeaponSniperRifle(){
    fire_rate=1500;
    last_fire=0;
    base_damage=65;
    bonus_rand_damage=20;  
}

CWeaponSniperRifle::~CWeaponSniperRifle(){

}

void CWeaponSniperRifle::Fire(vec2_t &pos, float angle, bool plane_left){
    Uint32 curtime=SDL_GetTicks(); 
    if(curtime>last_fire+fire_rate){
        sound_manager->PlaySound(sound_effects_ids::SNIPER_RIFLE_SHOT);
        last_fire=curtime;    
        angle*=M_PI/180.f;
        vbullets.push_back(SBullet(pos,angle));
    }
}

void CWeaponSniperRifle::Simulate(float delta){  
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
}

void CWeaponSniperRifle::Draw(vec2_t &plane_pos){
    SDL_SetRenderDrawColor(render->GetRenderer(),0,0,0,255);
    SDL_Rect bullet_rect;
    for(auto &bullet : vbullets){ 
        bullet_rect.x=(int)bullet.pos.x-2;
        bullet_rect.y=(int)bullet.pos.y-2;
        bullet_rect.w=4;
        bullet_rect.h=4;   
        SDL_RenderFillRect(render->GetRenderer(),&bullet_rect); 
    }  
} 

bool CWeaponSniperRifle::CheckCollision(vec2_t &plane_pos){
    float collision_radius=PLANE_RADIUS+2; 
    for(std::vector<SBullet>::iterator it = vbullets.begin(); it != vbullets.end(); ){ 
        float distance=plane_pos.Distance(it->pos);
        vec2_t next_pos = it->pos;
        next_pos.x+=cos(it->angle)*distance;
        next_pos.y+=sin(it->angle)*distance; 
        if (plane_pos.Distance(next_pos)<collision_radius) {
            it = vbullets.erase(it);
            return true;
        } else {
            ++it;
        }
    } 
    return false;
}

void CWeaponSniperRifle::Reset(){
    vbullets.clear();
    last_fire=0;
}
