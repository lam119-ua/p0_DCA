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

        void drawScore(int score, int posX, int posY);

    
    private:
        char entered_key;
        Bird player; //Jugador
        const float gravedad = 500.0f;
        bool showIntro = true;
        static bool introShownBefore;

        //Tamaño de las tuberías:
        int PIPE_W = 32;
        int PIPE_H = 320;
        float PIPE_SPEED = 100.0f;
        const float space = 30.0f;
        float birdScale = 1.5f;

        deque<PipePair> pipes;

        //Temporizador tuberías
        float spawnTimer = 0.0f;
        float spawnEvery = 2.0f;

        //Puntuación del jugador
        int score = 0;

        Texture2D birdSprite;
        Texture2D pipeSprite;
        Texture2D backgroundSprite;
        Texture2D scoreSprite[10];
        Texture2D introSprite;

        //Renderizado del movimiento
        Texture2D birdFrame[3]; //Array para las imagenes del pajaro
        int currentFrame = 0; //Frame que se esta mostrando 
        float frameTime = 0.0f; //Tiempo acumulado
        float frameSpeed = 0.1f; //Duración de cada frame

        float gap; //Espacio libre entre tuberias
        const float dificultadIncremento = 0.05f;

};