#pragma once

enum GAME_STATES{ 
    IN_GAME,
    AFTER_ROUND,
    AFTER_GAME,
    QUIT_GAME
};


class CGameState{
    int state;
public:
    CGameState();
    ~CGameState();

    void Set(int new_state);
    int Get();
}; 

extern CGameState* game_state;