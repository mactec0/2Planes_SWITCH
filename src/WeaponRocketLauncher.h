#pragma once
#include "Weapon.h"
#include <iostream>

class CWeaponRocketLauncher : public CWeapon{ 
    const float start_velocity = 350.f;
    const float max_velocity = 1700.f;
    const float acceleration = 2650.f;
    const float max_fall_velocity = 550.f; 
    vec2_t pos;
    float fall_velocity, velocity;
    float angle, fall_angle;
    bool active;

public: 
    CWeaponRocketLauncher();
    ~CWeaponRocketLauncher();

    void Fire(vec2_t &pos, float angle, bool plane_left);
    void Simulate(float delta);
    void Draw(vec2_t &plane_pos);
    void Reset();
    bool CheckCollision(vec2_t &plane_pos);
 
};
