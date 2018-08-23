#include "Weapon.h"

int CWeapon::GetDamage(){ 
    if(bonus_rand_damage==0)
        return base_damage;
    return base_damage+rand()%bonus_rand_damage; 
}