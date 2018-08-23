#include "GUI.h"


CGUI::CGUI(){ 
    visible=true;
    current_panel=PANELS::MAINMENU;

    panels.push_back(new CPanel(PANELS::MAINMENU,490,300,(void*)&MainMenuCallback));
    panels.push_back(new CPanel(PANELS::SETTINGS,490,500,(void*)&SettingsCallback)); 
    panels.push_back(new CPanel(PANELS::PLAY_SELECT,490,40)); 
    panels.push_back(new CPanel(PANELS::PAUSE,490,320));

    panels[PANELS::MAINMENU]->AddEntry(new CEntryButton("PLAY", ELEMENTS::PLAY_BUTTON));
    panels[PANELS::MAINMENU]->AddEntry(new CEntryButton("CONTROLS", ELEMENTS::CONTROLS_BUTTON));
    panels[PANELS::MAINMENU]->AddEntry(new CEntryButton("QUIT", ELEMENTS::QUIT_BUTTON));


    std::string *control_modes = new std::string[2];
	control_modes[0] = "TWO CONTROLLERS";
	control_modes[1] = "JOY CONS"; 
    panels[PANELS::SETTINGS]->AddEntry(new CEntryList("CONTROL MODE:",ELEMENTS::CONTROLLER_MODE_LIST, control_modes,2));
    panels[PANELS::SETTINGS]->AddEntry(new CEntryButton("BACK TO MAINMENU", ELEMENTS::MAINMENU_BUTTON));

    

 
	std::string *map_list = new std::string[2];
	map_list[0] = "SNOWY";
	map_list[1] = "GREEN";
    panels[PANELS::PLAY_SELECT]->AddEntry(new CEntryList("MAP SELECTION:",ELEMENTS::MAP_SELECTION_LIST, map_list,2));

    std::string *lives_list = new std::string[4];
	lives_list[0] = "1";
	lives_list[1] = "3";
	lives_list[2] = "6";
	lives_list[3] = "9";
    panels[PANELS::PLAY_SELECT]->AddEntry(new CEntryList("MAX LIVES:",ELEMENTS::LIVES_SELECTION_LIST, lives_list,4,1));

    std::string *wpn_list = new std::string[4];
	wpn_list[0] = "MACHINE GUN";
	wpn_list[1] = "RPG";
	wpn_list[2] = "SNIPER RIFLE";
	wpn_list[3] = "LASER GUN";
    panels[PANELS::PLAY_SELECT]->AddEntry(new CEntryList("P1 PRIMARY WEAPON:",ELEMENTS::P1_WPN_SELECTION_1_LIST, wpn_list,4));
    panels[PANELS::PLAY_SELECT]->AddEntry(new CEntryList("P1 SECONDARY WEAPON:",ELEMENTS::P1_WPN_SELECTION_2_LIST, wpn_list,4,1));
    panels[PANELS::PLAY_SELECT]->AddEntry(new CEntryList("P2 PRIMARY WEAPON:",ELEMENTS::P2_WPN_SELECTION_1_LIST, wpn_list,4));
    panels[PANELS::PLAY_SELECT]->AddEntry(new CEntryList("P2 SECONDARY WEAPON:",ELEMENTS::P2_WPN_SELECTION_2_LIST, wpn_list,4,1));

    panels[PANELS::PLAY_SELECT]->AddEntry(new CEntryButton("START", ELEMENTS::START_BUTTON));


    panels[PANELS::PAUSE]->AddEntry(new CEntryButton("RETURN TO GAME", ELEMENTS::RETURN_TO_GAME_BUTTON));
    panels[PANELS::PAUSE]->AddEntry(new CEntryButton("BACK TO MAINMENU", ELEMENTS::MAINMENU_BUTTON));
   

    list_to_remove.push_back(map_list);
    list_to_remove.push_back(lives_list);
    list_to_remove.push_back(wpn_list);
    list_to_remove.push_back(control_modes);

}

CGUI::~CGUI(){
    for(auto &to_remove:list_to_remove){
        delete [] to_remove;
    } 
}

bool CGUI::IsVisible(){
    return visible;
}

void CGUI::SetVisibility(bool visible){
    this->visible=visible;
}

void CGUI::Control(float delta){ 
    const float background_move=100.f;
    background_pos.x+=delta*background_move;
    background_pos.y+=delta*background_move;


    key_manager->SetController(0);

    int last_entry_ID=panels[current_panel]->GetCurrentEntryID();
    if(key_manager->IsClicked(joycon_keys::BUTTON_DPAD_UP)||
       key_manager->IsClicked(joycon_keys::BUTTON_LSTICK_UP)){
        panels[current_panel]->Previous();
    } 
    if(key_manager->IsClicked(joycon_keys::BUTTON_DPAD_DOWN)||
       key_manager->IsClicked(joycon_keys::BUTTON_LSTICK_DOWN)){
        panels[current_panel]->Next();
    }

    if(panels[current_panel]->GetCurrentEntryID()!=last_entry_ID)
        sound_manager->PlaySound(sound_effects_ids::MENU_SELECT);
    
    panels[current_panel]->Control(delta);

    switch(current_panel){
        case PANELS::MAINMENU:
            if(panels[PANELS::MAINMENU]->GetEntry(ELEMENTS::PLAY_BUTTON)->Clicked()){
                current_panel=PANELS::PLAY_SELECT;
            }
            if(panels[PANELS::MAINMENU]->GetEntry(ELEMENTS::CONTROLS_BUTTON)->Clicked()){
                current_panel=PANELS::SETTINGS;
            }
            if(panels[PANELS::MAINMENU]->GetEntry(ELEMENTS::QUIT_BUTTON)->Clicked()){
                game_state->Set(GAME_STATES::QUIT_GAME);
            }
        break; 
        case PANELS::SETTINGS: 
            key_manager->UseJoyCons(panels[PANELS::SETTINGS]->GetEntry(ELEMENTS::CONTROLLER_MODE_LIST)->GetValue());
            key_manager->ClearAll();
            if(panels[PANELS::SETTINGS]->GetEntry(ELEMENTS::MAINMENU_BUTTON)->Clicked()){
                current_panel=PANELS::MAINMENU;
            } 
        break;
        case PANELS::PLAY_SELECT:
            if(panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::START_BUTTON)->Clicked()){
                if(panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::P1_WPN_SELECTION_1_LIST)->GetValue()==
                  panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::P1_WPN_SELECTION_2_LIST)->GetValue()||
                  panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::P2_WPN_SELECTION_1_LIST)->GetValue()==
                  panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::P2_WPN_SELECTION_2_LIST)->GetValue()){
                      //error, you can't choose the same weapon twice
                }else{
                    current_panel=PANELS::PAUSE;
                    visible=false;
                    int lifes[]={1,3,6,9};
                    for(int i=0;i<PLANE_MAX;i++){
                        entities[i]->Reset();
                        entities[i]->SetLifes(lifes[panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::LIVES_SELECTION_LIST)->GetValue()]);
                        entities[i]->SetPrimaryWeapon(panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::P1_WPN_SELECTION_1_LIST+i*2)->GetValue());
                        entities[i]->SetSecondaryWeapon(panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::P1_WPN_SELECTION_2_LIST+i*2)->GetValue());
                    } 
                    sound_manager->PlaySound(sound_effects_ids::ROUND_BEGIN);
                    game_state->Set(GAME_STATES::IN_GAME); 
                    active_map=maps[panels[PANELS::PLAY_SELECT]->GetEntry(ELEMENTS::MAP_SELECTION_LIST)->GetValue()];

                }
            }
        break;
        case PANELS::PAUSE:
            for(int i=0;i<PLANE_MAX;i++)
                entities[i]->MuteEngine();
            
            if(panels[PANELS::PAUSE]->GetEntry(ELEMENTS::RETURN_TO_GAME_BUTTON)->Clicked()){
                visible=false;
            }
            if(panels[PANELS::PAUSE]->GetEntry(ELEMENTS::MAINMENU_BUTTON)->Clicked()){
                current_panel=PANELS::MAINMENU;
            }
        
        break;
    }
}

void CGUI::Draw(){
    DrawBackground(); 
    panels[current_panel]->Draw();
}

void CGUI::DrawBackground(){
    const int BACKGROUND_WIDTH=1280,BACKGROUND_HEIGHT=640;
    if(background_pos.x>BACKGROUND_WIDTH)
        background_pos.x-=BACKGROUND_WIDTH;
    if(background_pos.y>BACKGROUND_HEIGHT)
        background_pos.y-=BACKGROUND_HEIGHT;
    
    render->DrawTexture(texture_manager->GetTexture(MENU_BACKGROUND), background_pos);

    if(background_pos.y+BACKGROUND_HEIGHT<720){
        vec2_t next_background_pos=vec2_t(background_pos.x,background_pos.y+BACKGROUND_HEIGHT);
        render->DrawTexture(texture_manager->GetTexture(MENU_BACKGROUND), next_background_pos);
    } 
    if(background_pos.y>0){
        vec2_t next_background_pos=vec2_t(background_pos.x,background_pos.y-BACKGROUND_HEIGHT);
        render->DrawTexture(texture_manager->GetTexture(MENU_BACKGROUND), next_background_pos);
    }
    if(background_pos.x>0){
        vec2_t next_background_pos=vec2_t(background_pos.x-BACKGROUND_WIDTH,background_pos.y);
        render->DrawTexture(texture_manager->GetTexture(MENU_BACKGROUND), next_background_pos);
        if(next_background_pos.y+BACKGROUND_HEIGHT<720){
            next_background_pos=vec2_t(next_background_pos.x,next_background_pos.y+BACKGROUND_HEIGHT);
            render->DrawTexture(texture_manager->GetTexture(MENU_BACKGROUND), next_background_pos);
        }
    }
    if(background_pos.x>0&&background_pos.y>0){
        vec2_t next_background_pos=vec2_t(background_pos.x-BACKGROUND_WIDTH,background_pos.y-BACKGROUND_HEIGHT);
        render->DrawTexture(texture_manager->GetTexture(MENU_BACKGROUND), next_background_pos);
    } 
}

int CGUI::GetActivePanel(){
    return current_panel;
}

void CGUI::SetPause(){
    visible=true;
}

void CGUI::SetActivePanel(int panel){
    current_panel=panel;
}

CPanel::CPanel(int ID, int x, int y, void *callback){
    this->ID=ID;
    this->x=x;
    this->y=y;
    current_entry_id=0;
    custom_paint_callback=callback;
}

CPanel::~CPanel(){

}

void CPanel::Control(float delta){ 
    int entry_id=0;
    for(auto & entry: entries){
        entry->Control(current_entry_id==entry_id);
        entry_id++; 
    } 
}

void CPanel::Draw(){
    int draw_x=x,draw_y=y;
    int entry_id=0;
    for(auto & entry: entries){
        entry->Draw(current_entry_id==entry_id,draw_x,draw_y);
        entry_id++;
        draw_y+=100;
    }
    if(custom_paint_callback){
        typedef void (*custom_paint_callback_t)(CPanel *panel);
        custom_paint_callback_t paint_callback_call=(custom_paint_callback_t)custom_paint_callback;
        paint_callback_call(this);
    }
} 
CEntry *CPanel::GetEntry(int ID){
    for(auto & entry: entries){
        if(entry->ID==ID)
        return entry;
    } 
    return NULL;
}

void CPanel::AddEntry(CEntry* entry){
    entries.push_back(entry);
}

int CPanel::GetCurrentEntryID(){
    return current_entry_id;
}

void CPanel::Previous(){
    current_entry_id--;
    if(current_entry_id<0)
        current_entry_id=0; 
}

void CPanel::Next(){
    current_entry_id++;
    if(current_entry_id>=entries.size())
        current_entry_id=entries.size()-1; 
}

 
CEntryButton::CEntryButton(std::string text, int ID){
    this->text=text;
    this->ID=ID;
    clicked=false;
}

CEntryButton::~CEntryButton(){

}

void CEntryButton::Control(bool active){
    if(key_manager->IsClicked(joycon_keys::BUTTON_A)&&active){
        clicked=true;
        sound_manager->PlaySound(sound_effects_ids::MENU_CLICK);
    } 
}


void CEntryButton::Draw(bool active, int x, int y){
    SDL_SetRenderDrawColor(render->GetRenderer(),0,41,108,255);
    if(active)
        SDL_SetRenderDrawColor(render->GetRenderer(),0,31,77,255);

    SDL_Rect rect;
    rect.x=(int)x;
    rect.y=(int)y;
    rect.w=300;
    rect.h=42;   
    SDL_RenderFillRect(render->GetRenderer(),&rect); 

    SDL_Color color = {255, 255, 255};
    if(active)
        render->Printf(vec2_t(x+rect.w/2,y+rect.h/2), true, color, fonts_ids::MENU_1, (char*)"[%s]", text.c_str());
    else
        render->Printf(vec2_t(x+rect.w/2,y+rect.h/2), true, color, fonts_ids::MENU_1, (char*)"%s", text.c_str());

    SDL_SetRenderDrawColor(render->GetRenderer(),0,20,50,255);  
    for(int i=0;i<4;i++){
        rect.x--;
        rect.y--;
        rect.w+=2;
        rect.h+=2;
        SDL_RenderDrawRect(render->GetRenderer(),&rect);
    }
}

bool CEntryButton::Clicked(){
    if(clicked){
        clicked=false;
        return true;
    }
    return false;
}

int CEntryButton::GetValue(){
    return (int)clicked;
}


CEntryList::CEntryList(std::string text, int ID, std::string *list, int list_size,int start_value){
    this->text=text;
    this->ID=ID;
    this->list=list;
    this->list_size=list_size;
    value=start_value;
}

CEntryList::~CEntryList(){

}
 
void CEntryList::Control(bool active){
    int last_value = value;
    if(active&&(key_manager->IsClicked(joycon_keys::BUTTON_DPAD_RIGHT)||
                key_manager->IsClicked(joycon_keys::BUTTON_LSTICK_RIGHT))){
        value++;
    } 
    if(active&&(key_manager->IsClicked(joycon_keys::BUTTON_DPAD_LEFT)||
                key_manager->IsClicked(joycon_keys::BUTTON_LSTICK_LEFT))){
        value--;
    } 
    if(value<0)
        value=0;
    if(value>=list_size)
        value=list_size-1;

    if(value!=last_value){
        sound_manager->PlaySound(sound_effects_ids::MENU_SELECT2);
    }
}

void CEntryList::Draw(bool active, int x, int y){
    SDL_SetRenderDrawColor(render->GetRenderer(),0,41,108,255);
    if(active)
        SDL_SetRenderDrawColor(render->GetRenderer(),0,31,77,255);

    SDL_Rect rect;
    rect.x=(int)x;
    rect.y=(int)y;
    rect.w=300;
    rect.h=42;   
    SDL_RenderFillRect(render->GetRenderer(),&rect); 

    SDL_Color color = {255, 255, 255};
    if(active)
        render->Printf(vec2_t(x+rect.w/2,y+rect.h/2), true, color, fonts_ids::MENU_1, (char*)"< %s >", list[value].c_str());
    else
        render->Printf(vec2_t(x+rect.w/2,y+rect.h/2), true, color, fonts_ids::MENU_1, (char*)"%s", list[value].c_str());

    
    
    SDL_Color color2 = {140, 140, 140};
    render->Printf(vec2_t(x-5,y-25), false, color2, fonts_ids::MENU_2, (char*)"%s", text.c_str());

    SDL_SetRenderDrawColor(render->GetRenderer(),0,20,50,255);  
    for(int i=0;i<4;i++){
        rect.x--;
        rect.y--;
        rect.w+=2;
        rect.h+=2;
        SDL_RenderDrawRect(render->GetRenderer(),&rect);
    }
}

bool CEntryList::Clicked(){
    return false;
}

int CEntryList::GetValue(){
    return value;
}

