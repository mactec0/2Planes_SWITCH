#pragma once 
#include "vec2.h"
#include "Render.h"
#include "CustomPaintCallbacks.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "KeyManager.h"
#include "Landscape.h"
#include "GameState.h"
#include "Entity.h"
#include <vector>
#include <string>

enum PANELS{
    MAINMENU,
    SETTINGS,
    PLAY_SELECT, 
    PAUSE
};

enum  ELEMENTS{
    PLAY_BUTTON,
    CONTROLS_BUTTON,
    QUIT_BUTTON,

    CONTROLLER_MODE_LIST,

    START_BUTTON,
    MAP_SELECTION_LIST,
    LIVES_SELECTION_LIST,
    P1_WPN_SELECTION_1_LIST,
    P1_WPN_SELECTION_2_LIST,
    P2_WPN_SELECTION_1_LIST,
    P2_WPN_SELECTION_2_LIST,

    RETURN_TO_GAME_BUTTON,
    MAINMENU_BUTTON

};

class CEntry {
public:
    int ID; 
    virtual void Control(bool active) = 0;
    virtual void Draw(bool active, int x, int y) = 0;
    virtual bool Clicked() = 0;
    virtual int GetValue() = 0; 
};

class CEntryButton: public CEntry{
    bool clicked;
    std::string text;

public:
    CEntryButton(std::string text, int ID);
    ~CEntryButton();
 
    virtual void Control(bool active);
    virtual void Draw(bool active, int x, int y);
    virtual bool Clicked();
    virtual int GetValue(); 

};


class CEntryList: public CEntry{
    int value;
    std::string text;
	std::string *list;
    int list_size;

public:
    CEntryList(std::string text, int ID, std::string *list, int list_size, int start_value=0);
    ~CEntryList();
 
    virtual void Control(bool active);
    virtual void Draw(bool active, int x, int y);
    virtual bool Clicked();
    virtual int GetValue(); 

};

class CPanel{
    int ID;
    int x,y;
    int current_entry_id;
    std::vector<CEntry*> entries;
    void *custom_paint_callback;
public:
    CPanel(int ID, int x, int y, void *callback=NULL);
    ~CPanel();

    void AddEntry(CEntry* entry);
    void Control(float delta);
    void Draw(); 
    void Previous();
    void Next();
    int GetCurrentEntryID();
    CEntry *GetEntry(int ID);

};


class CGUI{  
    int current_panel;
    vec2_t background_pos;
    std::vector<CPanel*> panels;
    bool visible;

    std::vector<std::string*> list_to_remove;

    void DrawBackground();
public:

    CGUI();
    ~CGUI();

    void SetVisibility(bool visible);
    void SetPause();
    bool IsVisible(); 
    void Control(float delta);
    void Draw();
    void SetActivePanel(int panel);
    int GetActivePanel();

};

extern CGUI *gui;
