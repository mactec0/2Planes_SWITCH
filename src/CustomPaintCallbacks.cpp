#include "CustomPaintCallbacks.h"
#include "GUI.h"

void MainMenuCallback(CPanel *panel){
    render->Printf(vec2_t(SCREEN_WIDTH/2, 140), true, {255, 255, 255}, fonts_ids::MENU_LOGO, (char*)"2PLANES");
    render->Printf(vec2_t(20, 680), false, {255, 255, 255}, fonts_ids::MENU_1, (char*)"by mactec");
}


void SettingsCallback(CPanel *panel){
    vec2_t pos=vec2_t(200,80);
    render->DrawTexture(texture_manager->GetTexture(textures_ids::SETTINGS_0+panel->GetEntry(ELEMENTS::CONTROLLER_MODE_LIST)->GetValue()), pos);
}