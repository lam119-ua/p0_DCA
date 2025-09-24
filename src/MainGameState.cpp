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
    playerBox.x = player.x - 17.0f;
    playerBox.y = player.y - 17.0f;
    playerBox.height = 17.0f;
    playerBox.width = 17.0f;

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
    }

    //Comprobamos las colisiones con todas las tuberias
    for(size_t i = 0; i < pipes.size(); i++){
        if(CheckCollisionRecs(playerBox, pipes[i].top) || CheckCollisionRecs(playerBox, pipes[i].bot)){
            //Colision detectada
            this->state_machine->add_state(make_unique<GameOverState>(), true);
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
    DrawCircle(player.x, player.y, 17.0f, RED);

    for(size_t i = 0; i < pipes.size(); i++){

        //Tubería superior
        DrawRectangle(
            pipes[i].top.x,
            pipes[i].top.y,
            pipes[i].top.width,
            pipes[i].top.height,
            GREEN
        );

        //Tubería inferior
        DrawRectangle(
            pipes[i].bot.x,
            pipes[i].bot.y,
            pipes[i].bot.width,
            pipes[i].bot.height,
            GREEN
        );
    }

    //Finalizamos el render
    EndDrawing();
}