#include "GameState.h"


CGameState::CGameState(){
    state=GAME_STATES::IN_GAME;
}

CGameState::~CGameState(){

}

void CGameState::Set(int new_state){
    state=new_state;
}

int CGameState::Get(){
    return state;
}