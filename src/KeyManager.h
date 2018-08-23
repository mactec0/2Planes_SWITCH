#pragma once
#include <map>

#define joycon_left 0 
#define joycon_right 1

enum joycon_keys{
    BUTTON_A=0,    BUTTON_B,    BUTTON_X,    BUTTON_Y,
    BUTTON_LSTICK,    BUTTON_RSTICK,
    BUTTON_L,    BUTTON_R,
    BUTTON_ZL,    BUTTON_ZR,
    BUTTON_PLUS, BUTTON_MINUS,
    BUTTON_DPAD_LEFT,    BUTTON_DPAD_UP,    BUTTON_DPAD_RIGHT,    BUTTON_DPAD_DOWN,
    BUTTON_LSTICK_LEFT,    BUTTON_LSTICK_UP,    BUTTON_LSTICK_RIGHT,    BUTTON_LSTICK_DOWN,
    BUTTON_RSTICK_LEFT,    BUTTON_RSTICK_UP,    BUTTON_RSTICK_RIGHT,    BUTTON_RSTICK_DOWN,
    MAX_BUTTONS
};

struct SControllerRemap{
    SControllerRemap(){
        which=-1;
        key_id=-1;
    }
    SControllerRemap(int key_id, int which){
        this->which=which;
        this->key_id=key_id;
    }
    int which;
    int key_id;
};

class CKeyManager{ 
    bool keys_pressed[MAX_BUTTONS][2];
    bool keys_clicked[MAX_BUTTONS][2];
    int current_controller;
    bool use_joycons;
    std::map<int, SControllerRemap> to_joycon_key;
public:

	CKeyManager();
	~CKeyManager(); 

	bool IsClicked(int id);
	bool IsPressed(int id);

	void CleanPressed();

	void Set(int id, bool value, int which);

	void SetController(int id);

    void UseJoyCons(bool use);

    bool IsUsingJoyCons();

    void ClearAll();
};

extern CKeyManager *key_manager;