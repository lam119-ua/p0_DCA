extern "C" {
    #include <raylib.h>
}
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include <iostream>
using namespace std;

MainGameState::MainGameState(){
    
}

void MainGameState::init(){
    player.x = 200.0f;
    player.y = 200.0f;
    player.vy = 0.0f;

    //Cargar texturas
    birdSprite = LoadTexture("assets/redbird-midflap.png");
    pipeSprite = LoadTexture("assets/pipe-green.png");

    player.width = birdSprite.width;
    player.height = birdSprite.height;

    PIPE_H = pipeSprite.height;
    PIPE_W = pipeSprite.width;

    gap = player.height * 4.5f;
}

void MainGameState::handleInput(){
    if(IsKeyPressed(KEY_SPACE)){
        player.vy = -300.0f;
    }
}

void MainGameState::update(float deltaTime){

    //Actualizar jugador
    player.vy += gravedad * deltaTime;
    player.y += player.vy * deltaTime;
    //player.vy = 0.0f;

    //Bounding box del jugador
    Rectangle playerBox;
    playerBox.x = player.x - player.width / 2.0f;
    playerBox.y = player.y - player.height / 2.0f;
    playerBox.height = player.height;
    playerBox.width = player.width;

    //Temporizador de spawn
    spawnTimer += deltaTime;
    if(spawnTimer >= spawnEvery){
        spawnTimer = 0.0f;

        //Posición comienzo tuberías
        float x = GetScreenWidth();

        int min = PIPE_H / 2;
        int max = x / 2.0f;
        int pipe_y_offset_top = GetRandomValue(min, max);
        
        //Tubería superior
        Rectangle pipeTop;
        pipeTop.x = x;
        pipeTop.y = -pipe_y_offset_top;
        pipeTop.width = PIPE_W;
        pipeTop.height = PIPE_H;

        //Tubería Inferior
        Rectangle pipeBot;
        pipeBot.x = x;
        pipeBot.y = (PIPE_H - pipe_y_offset_top) + GetRandomValue(min, max);
        pipeBot.width = PIPE_W;
        pipeBot.height = PIPE_H;

        //Insertamos nuevo objeto en la cola
        PipePair pair;
        pair.top = pipeTop;
        pair.bot = pipeBot;
        pipes.push_back(pair);
    
    }

    //Mover las tuberías
    for(size_t i = 0; i < pipes.size(); i++){
        pipes[i].top.x -= PIPE_SPEED * deltaTime;
        pipes[i].bot.x -= PIPE_SPEED * deltaTime;

        if(!pipes[i].scored && pipes[i].top.x + PIPE_W < player.x){
            score++;
            pipes[i].scored = true;
        }
    }

    //Comprobamos las colisiones con todas las tuberias
    for(size_t i = 0; i < pipes.size(); i++){
        if(CheckCollisionRecs(playerBox, pipes[i].top) || CheckCollisionRecs(playerBox, pipes[i].bot)){
            //Colision detectada
            this->state_machine->add_state(make_unique<GameOverState>(score), true);
        }
    }

    //Comprobar si las tuberísa salen de la pantalla
    //PIPE_W < 0 comprueba si ha salido por la izquierda
    while(!pipes.empty() && pipes.front().top.x + PIPE_W < 0){
        pipes.pop_front();
    }
}

void MainGameState::render(){
    
    BeginDrawing();

    //Limpiamos todo lo dibujado
    ClearBackground(RAYWHITE);

    //Ponemos texto en pantalla
    DrawText("Bienvenido a Flappy Bird DCA", 20, 20, 20, BLACK);

    //Dibujamos el pajaro
    DrawTexture(birdSprite, player.x - player.width / 2, player.y - player.height / 2, WHITE);

    for(size_t i = 0; i < pipes.size(); i++){
        PipePair &p = pipes[i];

        // Tubería superior (rota 180°)
        DrawTextureEx(pipeSprite, {p.top.x + PIPE_W, p.top.y + PIPE_H}, 180.f, 1.0f, WHITE);

        // Tubería inferior
        DrawTextureEx(pipeSprite, {p.bot.x, p.bot.y}, 0.f, 1.0f, WHITE);
        
    }
    
    // void DrawText(const char *text, int posX, int posY, int fontSize, Color color); 
    DrawText(to_string(score).c_str(), 60, 60, 30, DARKBLUE);

    //Finalizamos el render
    EndDrawing();
}