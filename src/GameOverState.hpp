#pragma once
#include <iostream>
#include "GameOverState.hpp"
#include "GameState.hpp"
using namespace std;

class GameOverState: public GameState
{
    public:
        GameOverState(int finalScore);
        ~GameOverState() override = default;
        
        //Sobreescribimos los metodos virtuales
        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;
        void pause() override;
        void resume() override;
    
    private:
        int score;
};