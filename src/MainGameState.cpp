extern "C" {
    #include <raylib.h>
}
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include <iostream>
#include <cmath>
using namespace std;

bool MainGameState::introShownBefore = false;

MainGameState::MainGameState(){
    
}

void MainGameState::init(){
    if (introShownBefore) {
        showIntro = false;  // Si ya la mostraste, no se vuelve a mostrar
    } else {
        showIntro = true;   // Solo la primera vez
    }
    
    player.x = 142.0f;
    player.y = 283.0f;
    player.vy = 0.0f;

    //Cargar texturas
    //birdSprite = LoadTexture("assets/redbird-midflap.png");
    pipeSprite = LoadTexture("assets/pipe-green.png");
    backgroundSprite = LoadTexture("assets/background-day.png");
    introSprite = LoadTexture("assets/message.png");

    //Cargamos los 3 frames del pajaro
    birdFrame[0] = LoadTexture("assets/redbird-downflap.png");
    birdFrame[1] = LoadTexture("assets/redbird-midflap.png");
    birdFrame[2] = LoadTexture("assets/redbird-upflap.png");
    birdSprite = birdFrame[1];

    //Cargamos todos los numeros
    for(int i = 0; i < 10; i++){
        string path = "assets/" + to_string(i) + ".png";
        scoreSprite[i] = LoadTexture(path.c_str());
    }

    player.width = birdSprite.width * birdScale;
    player.height = birdSprite.height * birdScale;

    PIPE_H = pipeSprite.height;
    PIPE_W = pipeSprite.width;

    gap = player.height * 4.5f;
    PIPE_SPEED = 100.0f;
}

void MainGameState::handleInput(){
    if(showIntro && IsKeyPressed(KEY_SPACE)){
        showIntro = false;
        introShownBefore = true;
    }else{
        if (!showIntro && IsKeyPressed(KEY_SPACE)){
            player.vy = -300.0f;
        }
    }
}

void MainGameState::update(float deltaTime){

    if(!showIntro){
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

            int pipeY = GetRandomValue(50, GetScreenHeight() - gap - 50);
            int min = PIPE_H / 2;
            int max = x / 2.0f;
            int pipe_y_offset_top = GetRandomValue(min, max);
            
            //Tubería superior
            Rectangle pipeTop;
            pipeTop.x = x;
            pipeTop.y = pipeY - PIPE_H;
            pipeTop.width = PIPE_W;
            pipeTop.height = PIPE_H;

            //Tubería Inferior
            Rectangle pipeBot;
            pipeBot.x = x;
            pipeBot.y = pipeY + gap;
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

        //Incrementar dificultad poco a poco
        gap -= 2.0f * deltaTime; //Reducir el gap poco a poco
        if(gap < 100.0f){
            gap = 100.0f; //Gap minimo para que no sea imposible
        }

        PIPE_SPEED += dificultadIncremento * deltaTime * 100.0f; //Aumenta la velocidad
        if(PIPE_SPEED > 400.0f){
            PIPE_SPEED = 400.0f; //Limitamos la velocidad
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

        frameTime += deltaTime;

        if(frameTime >= frameSpeed){
            frameTime = 0.0f; //Reinicial tiempo acumulado
            currentFrame++; //Avanzar frame

            if(currentFrame > 2) currentFrame = 0;
        }
    }
}

void MainGameState::drawScore(int score, int posX, int posY){
    string scoreSTR = to_string(score);

    int digitWidth = scoreSprite[0].width;
    int totalWidth = digitWidth * scoreSTR.length();

    int centrar =  posX - totalWidth / 2;

    for(size_t i = 0; i < scoreSTR.length(); i++){
        int digit = scoreSTR[i] - '0'; //convertir char a numero
        DrawTexture(scoreSprite[digit], centrar + i * digitWidth, posY, WHITE);
    }
}

void MainGameState::render(){
    
    BeginDrawing();

    //Limpiamos todo lo dibujado
    ClearBackground(RAYWHITE);

    DrawTexture(backgroundSprite, 0, 0, WHITE);

    //Ponemos texto en pantalla
    //DrawText("Bienvenido a Flappy Bird DCA", 20, 20, 20, BLACK);

    //Dibujamos el pajaro
    float drawX = player.x - (player.width / 2);
    float drawY = player.y - (player.height / 2);

    //Dibujo el pajaro segun el frame
    DrawTextureEx(birdFrame[currentFrame], {drawX, drawY}, 0.0f, birdScale, WHITE);


    if(showIntro && !introShownBefore){
        float scaleX = (float)GetScreenWidth() / introSprite.width;
        float scaleY = (float)GetScreenHeight() / introSprite.height;
        float scale = fmin(scaleX, scaleY);

        DrawTextureEx(introSprite, {0, 0}, 0.0f, scale, WHITE);
    }

    for(size_t i = 0; i < pipes.size(); i++){
        PipePair &p = pipes[i];

        // Tubería superior (rota 180°)
        DrawTextureEx(pipeSprite, {p.top.x + PIPE_W, p.top.y + PIPE_H}, 180.f, 1.0f, WHITE);

        // Tubería inferior
        DrawTextureEx(pipeSprite, {p.bot.x, p.bot.y}, 0.f, 1.0f, WHITE);
        
    }
    
    drawScore(score, GetScreenWidth() / 2, 100);
    //DrawText(to_string(score).c_str(), 60, 60, 30, DARKBLUE);

    //Finalizamos el render
    EndDrawing();
}