#include "KeyManager.h"


CKeyManager::CKeyManager(){
	current_controller=0;
	use_joycons=false;
	for(int i=0;i<MAX_BUTTONS;i++){
		to_joycon_key[i].key_id=-1;
		keys_pressed[i][joycon_left]=false;
		keys_clicked[i][joycon_left]=false;
		keys_pressed[i][joycon_right]=false;
		keys_clicked[i][joycon_right]=false;
	}

	to_joycon_key[BUTTON_DPAD_DOWN]=SControllerRemap(joycon_keys::BUTTON_A, joycon_left);
	to_joycon_key[BUTTON_DPAD_LEFT]=SControllerRemap(joycon_keys::BUTTON_B, joycon_left);
	to_joycon_key[BUTTON_DPAD_UP]=SControllerRemap(joycon_keys::BUTTON_Y, joycon_left);
	to_joycon_key[BUTTON_LSTICK_UP]=SControllerRemap(joycon_keys::BUTTON_DPAD_LEFT, joycon_left);
	to_joycon_key[BUTTON_LSTICK_DOWN]=SControllerRemap(joycon_keys::BUTTON_DPAD_RIGHT, joycon_left);
	to_joycon_key[BUTTON_LSTICK_LEFT]=SControllerRemap(joycon_keys::BUTTON_DPAD_DOWN, joycon_left);
	to_joycon_key[BUTTON_LSTICK_RIGHT]=SControllerRemap(joycon_keys::BUTTON_DPAD_UP, joycon_left);

	to_joycon_key[BUTTON_X]=SControllerRemap(joycon_keys::BUTTON_A, joycon_right);
	to_joycon_key[BUTTON_A]=SControllerRemap(joycon_keys::BUTTON_B, joycon_right);
	to_joycon_key[BUTTON_B]=SControllerRemap(joycon_keys::BUTTON_Y, joycon_right);
	to_joycon_key[BUTTON_RSTICK_UP]=SControllerRemap(joycon_keys::BUTTON_DPAD_LEFT, joycon_right);
	to_joycon_key[BUTTON_RSTICK_DOWN]=SControllerRemap(joycon_keys::BUTTON_DPAD_RIGHT, joycon_right);
	to_joycon_key[BUTTON_RSTICK_LEFT]=SControllerRemap(joycon_keys::BUTTON_DPAD_UP, joycon_right);
	to_joycon_key[BUTTON_RSTICK_RIGHT]=SControllerRemap(joycon_keys::BUTTON_DPAD_DOWN, joycon_right);


	to_joycon_key[BUTTON_PLUS]=SControllerRemap(joycon_keys::BUTTON_PLUS, joycon_left);
	to_joycon_key[BUTTON_MINUS]=SControllerRemap(joycon_keys::BUTTON_MINUS, joycon_left);
}
 

CKeyManager::~CKeyManager(){

}


void CKeyManager::SetController(int id){
	current_controller=id;
}

bool CKeyManager::IsClicked(int id){
	return keys_clicked[id][current_controller];
}

bool CKeyManager::IsPressed(int id){
	return keys_pressed[id][current_controller];
}

void CKeyManager::CleanPressed(){ 
	for(int i=0;i<MAX_BUTTONS;i++){ 
		keys_clicked[i][0]=false;
		keys_clicked[i][1]=false;
	}
}

void CKeyManager::Set(int id, bool value, int which){
	if(which<0)
		which=0;
	if(which>=2)
		which=1;

	if(use_joycons){
		which=to_joycon_key[id].which;
		id=to_joycon_key[id].key_id;
	}else if(which==joycon_right){
		if(id==BUTTON_LSTICK_LEFT)
			id=BUTTON_LSTICK_RIGHT;
		else if(id==BUTTON_LSTICK_RIGHT)
			id=BUTTON_LSTICK_LEFT;

		if(id==BUTTON_DPAD_LEFT)
			id=BUTTON_DPAD_RIGHT;
		else if(id==BUTTON_DPAD_RIGHT)
			id=BUTTON_DPAD_LEFT;
	}
	
	keys_pressed[id][which]=value;
	if(value)
		keys_clicked[id][which]=true;
}

void CKeyManager::UseJoyCons(bool use){
	use_joycons=use;		
}

bool CKeyManager::IsUsingJoyCons(){
	return use_joycons;
}


void CKeyManager::ClearAll(){
	for(int i=0;i<MAX_BUTTONS;i++){ 
		keys_clicked[i][joycon_left]=false;
		keys_clicked[i][joycon_right]=false;
		keys_pressed[i][joycon_left]=false;
		keys_pressed[i][joycon_right]=false;
	} 
}