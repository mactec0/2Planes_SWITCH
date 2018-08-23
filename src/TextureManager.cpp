#include "TextureManager.h"


CTextureManager::CTextureManager(){
	initialized=false; 
 
	if(!LoadTexture("romfs:/Plane1.png",textures_ids::PLANE_1))
		return;
	if(!LoadTexture("romfs:/Plane2.png",textures_ids::PLANE_2))
		return;
	if(!LoadTexture("romfs:/menu_background.png",textures_ids::MENU_BACKGROUND))
		return;
	if(!LoadTexture("romfs:/background1.png",textures_ids::BACKGROUND_1))
		return;
	if(!LoadTexture("romfs:/background2.png",textures_ids::BACKGROUND_2))
		return;
	if(!LoadTexture("romfs:/BOMB.png",textures_ids::BOMB))
		return;
	for(int i=0;i<14;i++){
		std::string fname="romfs:/crash_"+std::to_string(i)+".png";
		if(!LoadTexture(fname.c_str(),textures_ids::CRASH_0+i))
			return; 
		
	}
	for(int i=0;i<6;i++){
		std::string fname="romfs:/smoke_"+std::to_string(i)+".png";
		if(!LoadTexture(fname.c_str(),textures_ids::SMOKE_0+i))
			return; 
	}
	if(!LoadTexture("romfs:/heart.png",textures_ids::HEART))
		return; 
	if(!LoadTexture("romfs:/settings_0.png",textures_ids::SETTINGS_0))
		return; 
	if(!LoadTexture("romfs:/settings_1.png",textures_ids::SETTINGS_1))
		return; 

	
	initialized=true;


}

CTextureManager::~CTextureManager(){ 
	for(int i=0;i<TEX_MAX;i++){
		SDL_DestroyTexture(textures[i]);
		SDL_FreeSurface(surfaces[i]);
	}
}

bool CTextureManager::IsInitialized(){
	return this->initialized;
}

SDL_Surface *CTextureManager::LoadSurface( const char* path )
{
    SDL_Surface* loadedSurface = IMG_Load( path );
    return loadedSurface;
}


SDL_Texture* CTextureManager::LoadTexture(const char* path, int ID){

	surfaces[ID] = LoadSurface(path);
	if(!surfaces[ID])
		return NULL; 
    textures[ID] = SDL_CreateTextureFromSurface(render->GetRenderer(), surfaces[ID]);
    return textures[ID];
}


SDL_Texture* CTextureManager::GetTexture(int ID){
	return textures[ID];
}

SDL_Surface* CTextureManager::GetSurface(int ID){ 
	return surfaces[ID];
}

