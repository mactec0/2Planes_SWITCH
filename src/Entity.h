#pragma once
#include "vec2.h"
#include "Render.h"
#include "KeyManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include <SDL2/SDL.h> 
#include <cmath>
#include <stdio.h>


#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

#include "Timer.h"
#include "WeaponMachineGun.h"
#include "WeaponRocketLauncher.h"
#include "WeaponSniperRifle.h"
#include "WeaponLaser.h"


#define PLANE_SIZE 42 
#define PLANE_RADIUS PLANE_SIZE/2

#define GROUND 70
#define PLANE_MAX 2
 

enum{
    STATE_NORMAL,
    STATE_CRASHED,
    STATE_RESPAWN,
};

enum{
    PLANE_RED,
    PLANE_BLUE
};
 

struct SSmokePart{
    SSmokePart(vec2_t pos,
               int tex_id, 
               Uint32 born_time){
        this->pos=pos; 
        this->tex_id=tex_id; 
        this->born_time=born_time; 

        this->pos.y-=32;
        this->pos.x-=32;
        
    }
    vec2_t pos;
    int tex_id;
    float angle;
    Uint32 born_time;
};

class CEntity{  
    const float max_speed=0.5;
    const float accel=0.00015;
    const float gravity=0.4;
    vec2_t pos;
    float angle;
    float velocity;
    bool on_ground;
    int id;
    int state;
    CWeapon *weapons[WEAPON_MAX];
    CWeapon *primary_weapon, *secondary_weapon;
    int health, lifes;
    bool plane_left; 

    bool spawn_protection;
    Timer spawn_protect_timer; 

    bool round_begin_lock;
    Timer round_begin_lock_timer; 

    bool destroyed;
    Timer crash_timer;
 
    Timer smoke_timer;
    std::vector<SSmokePart> vsmoke;

    void ClampVelocity();

    void NormalizeAngle();

public:
    CEntity(int id);
    ~CEntity();

    void Reset();

    void Draw();

    void Control(float delta);
 
    void DrawHealth();

    bool CheckCollision(vec2_t &plane_pos, int &dmg);

    vec2_t GetPos();

    void Hurt(int dmg);

    void SetPrimaryWeapon(int weapon_ID);
    void SetSecondaryWeapon(int weapon_ID);

    void SpawnProtect();

    void Crash();

    bool IsDestroyed();

    bool IsProtected();

    void SetLifes(int lifes);

    int GetLifes();

    int GetHealth();

    void MuteEngine();
};

extern CEntity * entities[PLANE_MAX];

 
