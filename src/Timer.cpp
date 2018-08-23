#include "Timer.h"

Timer::Timer(){
    time_start=0;
    limit=0;
}

Timer::Timer(int limit){
    SetAlarm(limit);
}

Timer::~Timer(){

}

void Timer::SetAlarm(int limit){
    this->limit=limit;
}

void Timer::Start(){
    time_start=SDL_GetTicks();
}

bool Timer::Alarm(){
    return (time_start+limit<SDL_GetTicks());
}

Uint32 Timer::GetElapsed(){
    return SDL_GetTicks()-time_start;
}