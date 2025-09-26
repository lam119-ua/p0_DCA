#pragma once
#include "GameState.hpp"
#include <deque>
using namespace std;

struct Bird {
        float x;
        float y;
        float vy; //Velocidad vertical
        float height; //Alto del sprite
        float width; //Ancho del sprite
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
        int PIPE_W = 32;
        int PIPE_H = 320;
        const float PIPE_SPEED = 100.0f;
        const float space = 30.0f;

        deque<PipePair> pipes;

        //Temporizador tuberías
        float spawnTimer = 0.0f;
        float spawnEvery = 2.0f;

        //Puntuación del jugador
        int score = 0;

        Texture2D birdSprite;
        Texture2D pipeSprite;

        float gap; //ESapcio libre entre tuberias

};