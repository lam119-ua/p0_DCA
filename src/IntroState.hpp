#pragma once
#include "IntroState.hpp"
#include "GameState.hpp"
#include <iostream>
extern "C" {
    #include <raylib.h>
}
using namespace std;

class IntroState : public GameState
{
    public:
        IntroState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;
        void pause() override;
        void resume() override;
    
    private:
        Texture2D introSprite;
};