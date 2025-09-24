extern "C" {
    #include <raylib.h>
}
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include <iostream>

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

    EndDrawing();
}

void GameOverState::pause(){

}

void GameOverState::resume(){
    
}