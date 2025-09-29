extern "C" {
    #include <raylib.h>
}
#include "IntroState.hpp"
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include <iostream>
using namespace std;

void IntroState::init(){
    introSprite = LoadTexture("assets/message.png");
}

void IntroState::handleInput(){
    if(IsKeyPressed(KEY_SPACE)){
        this->state_machine->add_state(make_unique<MainGameState>(), true);
    }
}

void IntroState::update(float deltaTime){

}

void IntroState::render(){
    
    BeginDrawing();
    
    //ClearBackground(RAYWHITE);
    
    DrawTexture(introSprite, 0, 0, WHITE);
    
    EndDrawing();
}

void IntroState::pause(){
    
}

void IntroState::resume(){

}