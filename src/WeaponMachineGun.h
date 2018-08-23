#pragma once
#include "Weapon.h"
#include <iostream>

class CWeaponMachineGun : public CWeapon{
    std::vector<SBullet> vbullets; 
    const float velocity=0.72f;
 
    const float max_overheat_time=3000.f;
    const float overheat_fire_rate_loss=200.f;
    float current_overheat;
    bool fired_this_tick;

public: 
    CWeaponMachineGun();
    ~CWeaponMachineGun();

    void Fire(vec2_t &pos, float angle, bool plane_left);
    void Simulate(float delta);
    void Draw(vec2_t &plane_pos);
    void Reset();
    bool CheckCollision(vec2_t &plane_pos);

    void SetOverHeatColor();
};
