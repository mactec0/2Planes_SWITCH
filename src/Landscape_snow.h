#pragma once 
#include <algorithm> 
#include <vector> 
#include <cctype>
#include "Landscape.h"
#include "Timer.h"
#include "vec2.h"

#define MAX_PARTICLES 256

#define LANDSCAPE_SNOW 0

struct SnowParticle{
    vec2_t Pos;
    float size; 
    float velocity;
};


class LandscapeSnow :public Landscape{ 
    std::vector<SnowParticle> vParticles;
    void *background_raw;
    Timer snow_particles_timer;

public:

	LandscapeSnow();
	~LandscapeSnow();

	virtual void DrawBackground();
	virtual void DrawForeground();

	virtual void Control(float delta);
 
};
