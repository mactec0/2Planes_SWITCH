#include "Entity.h"

 

CEntity::CEntity(int id){
	this->id=id;
    plane_left=id%2;
    weapons[WEAPON_MACHINEGUN] = new CWeaponMachineGun();
    weapons[WEAPON_ROCKETLAUNCHER] = new CWeaponRocketLauncher();
    weapons[WEAPON_SNIPERRIFLE] = new CWeaponSniperRifle();
    weapons[WEAPON_LASER] = new CWeaponLaser(); 
    primary_weapon=weapons[WEAPON_MACHINEGUN];
    secondary_weapon=weapons[WEAPON_ROCKETLAUNCHER];
    destroyed=false;

    spawn_protect_timer.SetAlarm(12000);
    round_begin_lock_timer.SetAlarm(5000);
    crash_timer.SetAlarm(392);
    smoke_timer.SetAlarm(25);

    lifes=0;
 
    sound_manager->PlaySound(sound_effects_ids::ENGINE_1+plane_left,-1);

	Reset();
}

CEntity::~CEntity(){
    for(int i=0;i<4;i++)
        delete weapons[i];
}


void CEntity::Reset(){ 
    angle = 0.f;
    velocity = 0.f;
    state = STATE_NORMAL;
    on_ground=true;
    health=100;

    pos.y=SCREEN_HEIGHT-GROUND-PLANE_SIZE;
    pos.x=(plane_left)?40:(SCREEN_WIDTH-40);

    SpawnProtect();

    round_begin_lock=true;
    round_begin_lock_timer.Start(); 
 
    sound_manager->SetVolume(sound_effects_ids::ENGINE_1+plane_left,0);

    primary_weapon->Reset();
    secondary_weapon->Reset();
    vsmoke.clear();
}


void CEntity::ClampVelocity(){
    if(velocity>max_speed)
        velocity=max_speed;
        
    if (velocity<0)
        velocity=0;       
}


void CEntity::NormalizeAngle(){
    if(angle>360.f)
        angle-=360.f;
    if(angle<0)
        angle+=360.f;
}



void CEntity::Draw(){ 

    if(state==STATE_CRASHED){ 
        auto elapsed=crash_timer.GetElapsed();
        if(!crash_timer.Alarm()){
            int tex_id=floor(elapsed/28); 
            vec2_t crash_pos=pos+vec2_t(-32,-24);
            render->DrawTexture(texture_manager->GetTexture(textures_ids::CRASH_0+tex_id), crash_pos);
        }else{
            destroyed=true;
        }
    }else if(state==STATE_NORMAL){

        primary_weapon->Draw(pos);
        secondary_weapon->Draw(pos);
    	render->DrawPlaneTexture(texture_manager->GetTexture(textures_ids::PLANE_1+id-1), pos, angle);

        //crosshair
        SDL_SetRenderDrawColor(render->GetRenderer(),0,0,0,255);
        vec2_t cross_pos=pos;
        float cross_dist=35.f+(velocity*180.f);
        cross_pos.y+=sin((plane_left?angle:angle+180.f)*M_PI/180.f)*cross_dist;
        cross_pos.x+=cos((plane_left?angle:angle+180.f)*M_PI/180.f)*cross_dist; 
        
        SDL_Rect cross_rect;
        cross_rect.x=(int)cross_pos.x-4;
        cross_rect.y=(int)cross_pos.y;
        cross_rect.w=9;
        cross_rect.h=1;   
        SDL_RenderFillRect(render->GetRenderer(),&cross_rect); 
        cross_rect.x=(int)cross_pos.x;
        cross_rect.y=(int)cross_pos.y-4;
        cross_rect.w=1;
        cross_rect.h=9;   
        SDL_RenderFillRect(render->GetRenderer(),&cross_rect); 

        for(auto &smoke: vsmoke){
            float h=50-health;
            if(h>40.f)
                h=40.f;
            float max_alpha=255.f*(h/40.f);
            float diff=SDL_GetTicks()-smoke.born_time;
            if(diff>210.f)
                diff=210.f;
            float alpha=(1.f-(diff)/210.f)*max_alpha;
            if(alpha>max_alpha)
                alpha=max_alpha;
            SDL_SetTextureAlphaMod(texture_manager->GetTexture(textures_ids::SMOKE_0+smoke.tex_id),(int)alpha);
            render->DrawTexture(texture_manager->GetTexture(textures_ids::SMOKE_0+smoke.tex_id), smoke.pos);

        }

    }


    
}

 
void CEntity::Control(float delta){ 
    if(spawn_protection){ 
        int alpha=SDL_GetTicks()%800;
        if(alpha>400)
            alpha=800-alpha;
        alpha/=4; 
        SDL_SetTextureAlphaMod(texture_manager->GetTexture(textures_ids::PLANE_1+id-1),((int)alpha%200+55));
        
        if(spawn_protect_timer.Alarm()){
            spawn_protection=false;
            SDL_SetTextureAlphaMod(texture_manager->GetTexture(textures_ids::PLANE_1+id-1),255);
        }
    }

    if(round_begin_lock_timer.Alarm())
        round_begin_lock=false;
    

    if(state==STATE_NORMAL&&!round_begin_lock){

        if(health<50&&smoke_timer.Alarm()){
            vsmoke.push_back(SSmokePart(pos,rand()%6,SDL_GetTicks()));
            smoke_timer.Start();
        }

        vsmoke.erase(
        std::remove_if(vsmoke.begin(), vsmoke.end(),
            [](const SSmokePart & smoke) { return (smoke.born_time+210<SDL_GetTicks()); }),
        vsmoke.end());
           
        if(key_manager->IsPressed(joycon_keys::BUTTON_B)){  
            velocity+=accel*delta;
        }else{ 
            velocity-=(accel*delta)/2.f;
        } 
 
        ClampVelocity();

        sound_manager->SetVolume(sound_effects_ids::ENGINE_1+plane_left,velocity*200.f);
         
        if(key_manager->IsPressed(joycon_keys::BUTTON_DPAD_LEFT)||
           key_manager->IsPressed(joycon_keys::BUTTON_LSTICK_LEFT)){ 
            if(!on_ground||velocity>0.26){
                angle+=velocity*delta*(plane_left?-0.42:0.42); 
            }  
        }
            
        if(key_manager->IsPressed(joycon_keys::BUTTON_DPAD_RIGHT)||
           key_manager->IsPressed(joycon_keys::BUTTON_LSTICK_RIGHT)){
            if(!on_ground||velocity>0.26){
                angle+=velocity*delta*(plane_left?0.42:-0.42); 
            }
        }

        NormalizeAngle();
         
         
        float real_gravity=(gravity-velocity)*delta;
        if(real_gravity>=0)
            if(!on_ground)
                pos.y+=real_gravity;
        
        
        
        vec2_t vmove((delta*velocity)*cos(angle*M_PI/180.f), 
                    (delta*velocity)*sin(angle*M_PI/180.f));
        
         

        pos+=(plane_left)?vmove:-vmove; 
 
        if(pos.y+PLANE_SIZE>=SCREEN_HEIGHT-GROUND){
            pos.y=(SCREEN_HEIGHT-GROUND)-PLANE_SIZE;
            on_ground=true;
        }else{
            on_ground=false;
        }

        if(pos.y<-400.f)
            pos.y=-400.f;
        
        
        if(SCREEN_WIDTH<pos.x){
            pos.x=pos.x-SCREEN_WIDTH;
        } 
        if(pos.x+PLANE_SIZE<0){
            pos.x=SCREEN_WIDTH+pos.x;
        }
        
        if(plane_left?(on_ground&&angle<330&&angle>10):(on_ground&&angle<350&&angle>30)){ 
            sound_manager->SetVolume(sound_effects_ids::ENGINE_1+plane_left,0);
            Crash();
            return;  
        }
        
        
        if(on_ground&&velocity<0.18){
            if(plane_left){
                if(angle>330.f){
                    if(velocity!=0.f){
                        angle+=0.006f*delta*(1/velocity);
                    }else{
                        angle+=0.006f*delta*1.4f;
                    }       
                    if(angle<330.f)
                        angle=0.f;
                }
            }else{
                if(angle<30.f){
                    if(velocity!=0){
                        angle-=0.006f*delta*(1/velocity);
                    }else{
                        angle-=0.006f*delta*1.4f;
                    }
                    if(angle<0.f)
                        angle=0.f; 
                }
            }
        }

        primary_weapon->Simulate(delta);
        if(!spawn_protection&&!on_ground&&key_manager->IsPressed(joycon_keys::BUTTON_Y)){
            vec2_t plane_center=vec2_t(pos.x,pos.y+5); 
            primary_weapon->Fire(plane_center,plane_left?angle:angle+180.f,plane_left);
        }

        secondary_weapon->Simulate(delta);
        if(!spawn_protection&&!on_ground&&key_manager->IsPressed(joycon_keys::BUTTON_A)){
            vec2_t plane_center=vec2_t(pos.x,pos.y+5); 
            secondary_weapon->Fire(plane_center,plane_left?angle:angle+180.f,plane_left);
        }


        if(health<=0){
            Crash();  
        }
   } 
}

void CEntity::DrawHealth(){ 
    const int hbar_width=160;
    const int hbar_height=28;

    SDL_SetRenderDrawColor(render->GetRenderer(),0,0,0,255);
    SDL_Rect hbar_rect;
    hbar_rect.y=45;
    hbar_rect.w=hbar_width;
    hbar_rect.h=hbar_height; 
    hbar_rect.x=plane_left?12:(SCREEN_WIDTH-(hbar_width+12)); 
    SDL_RenderFillRect(render->GetRenderer(),&hbar_rect); 


    int lifes_start_x=plane_left?12:SCREEN_WIDTH-12;
    for(int i=0;i<lifes;i++){ 
        vec2_t heart_pos=vec2_t((plane_left?25*i:-25*i-25)+lifes_start_x,hbar_rect.y+38);
        render->DrawTexture(texture_manager->GetTexture(textures_ids::HEART), heart_pos);
    }


    hbar_rect.x+=2;
    hbar_rect.y+=2;
    hbar_rect.h-=4;
    hbar_rect.w-=4; 
    SDL_SetRenderDrawColor(render->GetRenderer(),200,0,0,255);
    SDL_RenderFillRect(render->GetRenderer(),&hbar_rect); 
 
    hbar_rect.w=(float)hbar_rect.w*((float)health/100.f);
    SDL_SetRenderDrawColor(render->GetRenderer(),0,200,0,255);
    SDL_RenderFillRect(render->GetRenderer(),&hbar_rect); 

}


bool CEntity::CheckCollision(vec2_t &plane_pos,int &dmg){
    if(primary_weapon&&primary_weapon->CheckCollision(plane_pos)){
        dmg=primary_weapon->GetDamage();
        return true;
    } 
    if(secondary_weapon&&secondary_weapon->CheckCollision(plane_pos)){
        dmg=secondary_weapon->GetDamage();
        return true;
    } 
    return false;
}

vec2_t CEntity::GetPos(){
    return pos;
}


void CEntity::Hurt(int dmg){
    health-=dmg;
    if(health<0)
        health=0;
}




void CEntity::SetPrimaryWeapon(int weapon_ID){ 
    primary_weapon=weapons[weapon_ID]; 
}

void CEntity::SetSecondaryWeapon(int weapon_ID){ 
    secondary_weapon=weapons[weapon_ID];
}


void CEntity::SpawnProtect(){
    spawn_protection=true;
    spawn_protect_timer.Start(); 
}

void CEntity::Crash(){
    crash_timer.Start();
    state=STATE_CRASHED;
    vsmoke.clear();
    lifes--;
    sound_manager->PlaySound(sound_effects_ids::EXPLOSION);
}

bool CEntity::IsDestroyed(){
    if(state==STATE_CRASHED&&destroyed)
        return true;
    return false;
}

bool CEntity::IsProtected(){
    return spawn_protection;
}


void CEntity::SetLifes(int lifes){
    this->lifes=lifes;
}


int CEntity::GetLifes(){
    return lifes;
}

int CEntity::GetHealth(){
    return health;
}
 
 
void CEntity::MuteEngine(){
    sound_manager->SetVolume(sound_effects_ids::ENGINE_1+plane_left,0); 
} 

