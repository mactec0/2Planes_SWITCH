#include "SoundManager.h"


CSoundManager::CSoundManager(){
	initialized=false;

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 )
		return;
 
	if(!LoadSound("romfs:/machine_gun.wav", sound_effects_ids::MACHINE_GUN_SHOT))
		return; 
	if(!LoadSound("romfs:/rocket.wav", sound_effects_ids::ROCKET_SHOT))
		return; 
	if(!LoadSound("romfs:/sniper_rifle.wav", sound_effects_ids::SNIPER_RIFLE_SHOT))
		return; 
	if(!LoadSound("romfs:/laser_beam.wav", sound_effects_ids::LASER_BEAM_SHOT))
		return; 
	if(!LoadSound("romfs:/menu_select.wav", sound_effects_ids::MENU_SELECT))
		return; 
	if(!LoadSound("romfs:/menu_select2.wav", sound_effects_ids::MENU_SELECT2))
		return; 
	if(!LoadSound("romfs:/menu_click.wav", sound_effects_ids::MENU_CLICK))
		return; 
	if(!LoadSound("romfs:/round_begin.wav", sound_effects_ids::ROUND_BEGIN))
		return; 
	if(!LoadSound("romfs:/round_end.wav", sound_effects_ids::ROUND_END))
		return; 
	if(!LoadSound("romfs:/explosion.wav", sound_effects_ids::EXPLOSION))
		return; 
	if(!LoadSound("romfs:/engine.wav", sound_effects_ids::ENGINE_1))
		return; 
	if(!LoadSound("romfs:/engine.wav", sound_effects_ids::ENGINE_2))
		return; 
   

	initialized=true;

}

bool CSoundManager::IsInitialized(){
	return this->initialized;
}

Mix_Chunk* CSoundManager::LoadSound(const char *path, int ID){
	sounds[ID] = Mix_LoadWAV(path);
	return sounds[ID];
}  



CSoundManager::~CSoundManager(){ 
	for(int i=0;i<SOUND_EFFECTS_MAX;i++)
		Mix_FreeChunk(sounds[i]);
}


int CSoundManager::PlaySound(int id, int loops){
	return Mix_PlayChannel( -1, sounds[id], loops);
}

void CSoundManager::StopSound(int id){
	Mix_HaltChannel(id);
}


void CSoundManager::SetVolume(int id,int volume){
	Mix_VolumeChunk(sounds[id],volume);
}