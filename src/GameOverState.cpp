extern "C" {
    #include <raylib.h>
}
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include <iostream>

GameOverState::GameOverState(int finalScore){
    score = finalScore;
}

void GameOverState::init(){

}

void GameOverState::handleInput(){
    if(IsKeyPressed(KEY_SPACE)){
        this->state_machine->add_state(make_unique<MainGameState>(), true);
    }
}

void GameOverState::update(float deltaTime){

}

void GameOverState::render(){
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Game Over", 75, 256, 30, BLACK);

    DrawText(to_string(score).c_str(), 144, 200, 30, DARKBLUE);

    EndDrawing();
}

void GameOverState::pause(){

}

void GameOverState::resume(){
    
}