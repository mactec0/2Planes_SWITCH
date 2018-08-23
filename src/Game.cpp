 #include "Game.h"

CGame::CGame(){ 
    after_death_timer.SetAlarm(4000);  
    winner=0; 
 

    Result rc = romfsInit();
    if (R_FAILED(rc)){
        game_state->Set(GAME_STATES::QUIT_GAME);
        return;
    } 

    game_state = new CGameState();

    key_manager = new CKeyManager();

    render = new CGameRender(); 
    if(!render->IsInitialized()){
        printf("render initialization error\n");
        game_state->Set(GAME_STATES::QUIT_GAME);
        return;
    }

    texture_manager = new CTextureManager(); 
    if(!texture_manager->IsInitialized()){
        printf("texture_manager initialization error\n");
        game_state->Set(GAME_STATES::QUIT_GAME);
        return;
    }


    font_manager = new CFontManager();
    if(!font_manager->IsInitialized()){
        printf("font_manager initialization error\n");
        game_state->Set(GAME_STATES::QUIT_GAME);
        return;
    }


    sound_manager = new CSoundManager(); 
    if(!sound_manager->IsInitialized()){
        printf("sound_manager initialization error\n");
        game_state->Set(GAME_STATES::QUIT_GAME);
        return;
    }


    gui = new CGUI();  
  
    for(int i=0; i<PLANE_MAX;i++)
        entities[i] = new CEntity(i+1);

    render->SetFramerateLimit(60);
   
    for (int i = 0; i < 2; i++) {
        if (SDL_JoystickOpen(i) == NULL) {
            printf("SDL_JoystickOpen: %s\n", SDL_GetError());
            game_state->Set(GAME_STATES::QUIT_GAME);
            return;
        }
    } 
    
    maps[LANDSCAPE_SNOW] = new LandscapeSnow(); 
    maps[LANDSCAPE_GREEN] = new LandscapeGreen(); 
    active_map = maps[LANDSCAPE_SNOW]; 

    last_time=SDL_GetTicks();
}

CGame::~CGame(){
    for(int i=0;i<PLANE_MAX;i++)
        delete entities[i];
    for(int i=0;i<NUMBER_OF_MAPS;i++)
        delete maps[i];
    delete game_state;
    delete key_manager;
    delete gui;
    delete render;
    delete font_manager;
    delete sound_manager;
    delete texture_manager;

}

void CGame::Run(){
    unsigned int dtime=SDL_GetTicks()-last_time;
    last_time=SDL_GetTicks();
    float fdtime=dtime;
    fdtime/=1000.f;

    render->BeginFrame();

    key_manager->CleanPressed();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if(event.type==SDL_JOYBUTTONUP)
            key_manager->Set(event.jbutton.button,false,event.jbutton.which);
            
        if(event.type==SDL_JOYBUTTONDOWN)
            key_manager->Set(event.jbutton.button,true,event.jbutton.which);
    }

    if(!gui->IsVisible()){

        for(int i=0;i<PLANE_MAX;i++){
            key_manager->SetController(i);
            entities[i]->Control((float)dtime);
        }

        active_map->Control(fdtime);
        
        active_map->DrawBackground();

        entities[0]->Draw();
        entities[1]->Draw();

        active_map->DrawForeground();

        entities[0]->DrawHealth();
        entities[1]->DrawHealth();

        for(int i=0;i<PLANE_MAX;i++){
            for(int j=0;j<PLANE_MAX;j++){
                if(entities[i]==entities[j])
                    continue; 
                if(entities[j]->IsProtected())
                    continue;
                int dmg=0;
                vec2_t pos =entities[j]->GetPos();
                if(entities[i]->CheckCollision(pos,dmg)){
                    entities[j]->Hurt(dmg);
                }
            }
        }

        
        key_manager->SetController(0);
        if(game_state->Get()==GAME_STATES::IN_GAME)
        if(key_manager->IsClicked(joycon_keys::BUTTON_PLUS)||
            key_manager->IsClicked(joycon_keys::BUTTON_MINUS)){ 
                gui->SetPause();
        }

        if(game_state->Get()==GAME_STATES::IN_GAME)
        for(int i=0;i<PLANE_MAX;i++){
            if(!entities[i]->IsDestroyed())
                continue;
            game_state->Set(GAME_STATES::AFTER_ROUND); 
            sound_manager->PlaySound(sound_effects_ids::ROUND_END);
            after_death_timer.Start(); 
            if(entities[i]->GetLifes()==0){
                game_state->Set(GAME_STATES::AFTER_GAME); 
                winner=i?1:2;
                break;
            } 
        }


        if(game_state->Get()==GAME_STATES::AFTER_ROUND&&after_death_timer.Alarm()){
            for(int i=0;i<PLANE_MAX;i++){
                entities[i]->Reset();
            }
            sound_manager->PlaySound(sound_effects_ids::ROUND_BEGIN);
            game_state->Set(GAME_STATES::IN_GAME); 
        }

        if(game_state->Get()==GAME_STATES::AFTER_GAME){
            SDL_Color color = {0, 0, 0};
            std::string end_message="PLAYER "+std::to_string(winner)+" WON!!!";
            render->Printf(vec2_t(SCREEN_WIDTH/2,SCREEN_HEIGHT/2), true, color, fonts_ids::MENU_3, (char*)"%s", end_message.c_str());
            
            if(after_death_timer.Alarm()){
                game_state->Set(GAME_STATES::IN_GAME); 
                gui->SetActivePanel(PANELS::MAINMENU);
                gui->SetVisibility(true); 
                for(int i=0;i<PLANE_MAX;i++)
                    entities[i]->MuteEngine(); 
            }

        } 

    }else{  
        gui->Control(fdtime);
        gui->Draw(); 
    }



    render->SwapBuffers();

}

bool CGame::Quit(){
    return (game_state->Get()==GAME_STATES::QUIT_GAME);
}
