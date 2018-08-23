#pragma once
#include <SDL2/SDL.h>

class Timer{
    Uint32 limit, time_start;
public:
    Timer();
    Timer(int limit);
    ~Timer();
    void SetAlarm(int limit);
    void Start();
    bool Alarm();
    Uint32 GetElapsed();
};