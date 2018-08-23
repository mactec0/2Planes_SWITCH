#include "FontManager.h"


CFontManager::CFontManager(){
	initialized=false;

	if(TTF_Init()==-1){
		return;
	}	 
 
    fonts[fonts_ids::MENU_1] = TTF_OpenFont("romfs:/lilliput_steps.ttf",24);
    fonts[fonts_ids::MENU_2] = TTF_OpenFont("romfs:/lilliput_steps.ttf",18);
    fonts[fonts_ids::MENU_3] = TTF_OpenFont("romfs:/lilliput_steps.ttf",32);
    fonts[fonts_ids::MENU_LOGO] = TTF_OpenFont("romfs:/lilliput_steps.ttf",56);
  

	for(int i=0;i<FONTS_MAX;i++){
		if(!fonts[i])
			return;
	}

	initialized=true;

}

bool CFontManager::IsInitialized(){
	return this->initialized;
}


CFontManager::~CFontManager(){

	for(int i=0;i<FONTS_MAX;i++)
		TTF_CloseFont(fonts[i]);
 
	TTF_Quit();

}


TTF_Font* CFontManager::LoadFont(const char *path, int ID, int size){
	fonts[ID] = TTF_OpenFont(path, size);
    if(!fonts[ID])
    	return NULL;
    return fonts[ID];
}

TTF_Font* CFontManager::GetFont(int ID){ 
	if(ID<0||ID>=fonts_ids::FONTS_MAX)
		return fonts[0];
	return fonts[ID];
}



