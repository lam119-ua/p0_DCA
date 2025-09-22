#pragma once
#include "GameState.hpp"
#include <deque>
using namespace std;

struct Bird {
        int x;
        int y;
        float vy; //Velocidad vertical
};

struct PipePair {
    Rectangle top, bot;
    bool scored = false;
};

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
        Bird player; //Jugador
        const float gravedad = 500.0f;

        //Tamaño de las tuberías:
        const int PIPE_W = 32;
        const int PIPE_H = 320;
        const float PIPE_SPEED = 100.0f;
        const float space = 30.0f;

        deque<PipePair> pipes;

        //Temporizador tuberías
        float spawnTimer = 0.0f;
        float spawnEvery = 2.0f;

};