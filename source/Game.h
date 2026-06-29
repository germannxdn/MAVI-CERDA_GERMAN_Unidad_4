#pragma once
#include "PhysicsWorld.h"

enum class GameState {
    Waiting,
    Charging,
    Running,
    Hit,
    Finished
};

class Game {
private:
    PhysicsWorld physicsWorld;
    GameState currentState;
    bool running;

    // Variables para la mira y carga
    float chargePower;        // 0.0 a 1.0
    float maxChargeTime;      // Tiempo máximo de carga (segundos)
    float currentChargeTime;  // Tiempo acumulado

public:
    Game();
    ~Game();

    void Run();
    void HandleInput();
    void Update();
    void Draw();
    void DrawUI();
    void DrawAim();

private:
    void ResetGame();
};