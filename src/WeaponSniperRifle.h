#pragma once
#include "Weapon.h"
#include <iostream>

class CWeaponSniperRifle : public CWeapon{
    std::vector<SBullet> vbullets; 
    const float velocity=2.92f; 
  
public: 
    CWeaponSniperRifle();
    ~CWeaponSniperRifle();

    void Fire(vec2_t &pos, float angle, bool plane_left);
    void Simulate(float delta);
    void Draw(vec2_t &plane_pos);
    void Reset();
    bool CheckCollision(vec2_t &plane_pos);
 
};
