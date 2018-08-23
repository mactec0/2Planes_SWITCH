#pragma once
#include "Weapon.h"
#include <iostream>

class CWeaponLaser : public CWeapon{ 
    bool active;
    vec2_t pos;
    float angle; 
    bool to_fire; 
    bool can_fire_again;
    Uint32 last_attack; 
    int sound_channel;
  
public: 
    CWeaponLaser();
    ~CWeaponLaser();

    void Fire(vec2_t &pos, float angle, bool plane_left);
    void Simulate(float delta);
    void Draw(vec2_t &plane_pos);
    void Reset();
    bool CheckCollision(vec2_t &plane_pos);
 
};
