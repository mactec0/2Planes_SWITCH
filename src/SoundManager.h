#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Render.h"

enum sound_effects_ids{
	MACHINE_GUN_SHOT,
	ROCKET_SHOT,
	SNIPER_RIFLE_SHOT,
	LASER_BEAM_SHOT,

	MENU_SELECT,
	MENU_SELECT2,
	MENU_CLICK,
	ROUND_BEGIN,
	ROUND_END,

	EXPLOSION,

	ENGINE_1,
	ENGINE_2,

	SOUND_EFFECTS_MAX
};

class CSoundManager{
	Mix_Chunk *sounds[sound_effects_ids::SOUND_EFFECTS_MAX]; 
	bool initialized; 

	Mix_Chunk* LoadSound(const char *path, int ID); 

public:

	CSoundManager();
	~CSoundManager();
	bool IsInitialized();

	int PlaySound(int id, int loops=0);

	void StopSound(int id);

	void SetVolume(int id,int volume); 
};

extern CSoundManager *sound_manager;
