extern "C" {
    #include <raylib.h>
}
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "IntroState.hpp"
#include <memory>
#include <chrono>

int main()
{ 
    float delta_time = 0.0f;

    //EJERCICIO 0
    InitWindow(288, 512, "FlappyBird");

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainGameState>(), false);
    state_machine.handle_state_changes(delta_time);

    while (!state_machine.is_game_ending())
    {
        delta_time = GetFrameTime();
        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();
    }

    CloseWindow();

    return 0;
}