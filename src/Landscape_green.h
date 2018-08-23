#pragma once 
#include <algorithm> 
#include <vector> 
#include <cctype>
#include "Landscape.h"
#include "Timer.h"
#include "vec2.h"


#define LANDSCAPE_GREEN 1

class LandscapeGreen :public Landscape{  
    void *background_raw; 

public:

	LandscapeGreen();
	~LandscapeGreen();

	virtual void DrawBackground();
	virtual void DrawForeground();

	virtual void Control(float delta);
 
};
