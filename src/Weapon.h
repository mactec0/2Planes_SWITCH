#pragma once
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include "vec2.h"
#include "Render.h"  
#include "SoundManager.h"
#include "TextureManager.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Timer.h"

enum{
    WEAPON_MACHINEGUN,
    WEAPON_ROCKETLAUNCHER,
    WEAPON_SNIPERRIFLE,
    WEAPON_LASER,
    WEAPON_MAX
}; 

struct SBullet{
    SBullet(vec2_t pos, float angle){
        this->pos=pos;
        this->angle=angle;
    }
    SBullet(){
        pos=vec2_t(0.f,0.f);
        angle=0.f;
    }
    vec2_t pos;
    float angle;
};

class CWeapon{
protected:
    Uint32 fire_rate, last_fire; 
    int base_damage, bonus_rand_damage;
public:

    CWeapon(){}
    virtual ~CWeapon(){};

    virtual void Fire(vec2_t &pos, float angle, bool plane_left = false) = 0;
    virtual void Simulate(float delta) = 0;
    virtual void Draw(vec2_t &plane_pos) = 0;
    virtual void Reset() = 0;
    virtual bool CheckCollision(vec2_t &plane_pos) = 0;
    int GetDamage();
};
