#include "Game.h"
#include "raylib.h"

Game::Game() : running(true), currentState(GameState::Waiting),
chargePower(0.0f), maxChargeTime(2.0f), currentChargeTime(0.0f) {
    InitWindow(1000, 600, "MAVI II - Unidad 4: Eventos y Colisiones");
    SetTargetFPS(60);
}

Game::~Game() {
    CloseWindow();
}

void Game::ResetGame() {
    physicsWorld.ResetScene();
    currentState = GameState::Waiting;
    chargePower = 0.0f;
    currentChargeTime = 0.0f;
}

void Game::HandleInput() {
    // ============================================
    // CARGA DE POTENCIA (manteniendo ESPACIO)
    // ============================================
    if (currentState == GameState::Waiting || currentState == GameState::Charging) {
        if (IsKeyDown(KEY_SPACE)) {
            // Si no estaba cargando, empezar
            if (currentState == GameState::Waiting) {
                currentState = GameState::Charging;
                currentChargeTime = 0.0f;
                chargePower = 0.0f;
            }

            // Aumentar potencia progresivamente
            currentChargeTime += GetFrameTime();
            chargePower = currentChargeTime / maxChargeTime;
            if (chargePower > 1.0f) chargePower = 1.0f;
        }

        // Soltar ESPACIO = disparar
        if (IsKeyReleased(KEY_SPACE) && currentState == GameState::Charging) {
            // Calcular fuerza basada en la potencia
            float forceX = 10.0f + chargePower * 100.0f;   // 10 a 110
            float forceY = -(8.0f + chargePower * 60.0f);  // -8 a -68

            physicsWorld.LaunchProjectile(150.0f, 500.0f, forceX, forceY);
            currentState = GameState::Running;
            chargePower = 0.0f;
            currentChargeTime = 0.0f;
        }
    }

    // Reiniciar con R
    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }

    // Si ya impactó, reiniciar con ESPACIO o R
    if (currentState == GameState::Hit || currentState == GameState::Finished) {
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_R)) {
            ResetGame();
        }
    }
}

void Game::Update() {
    physicsWorld.Update();

    if (currentState == GameState::Running && physicsWorld.HasProjectileHitTarget()) {
        currentState = GameState::Hit;
    }

    if (currentState == GameState::Running) {
        b2Vec2 pos = physicsWorld.GetProjectilePosition();
        if (pos.x > 1200 || pos.y > 700) {
            ResetGame();
        }
    }
}

void Game::DrawAim() {
    if (currentState != GameState::Waiting && currentState != GameState::Charging) return;

    // Posición de salida del proyectil
    float startX = 150.0f;
    float startY = 500.0f;

    // Dirección del disparo (fija hacia el objetivo)
    float targetX = 800.0f;
    float targetY = 460.0f;
    float dx = targetX - startX;
    float dy = targetY - startY;
    float length = sqrt(dx * dx + dy * dy);
    float dirX = dx / length;
    float dirY = dy / length;

    // Potencia actual
    float power = chargePower;
    float lineLength = 50.0f + power * 150.0f;  // 50 a 200 píxeles

    // Color según potencia (verde → amarillo → rojo)
    Color lineColor;
    if (power < 0.5f) {
        float t = power / 0.5f;
        lineColor = { (unsigned char)(0 + t * 255), 255, (unsigned char)(255 - t * 255), 255 };
    }
    else {
        float t = (power - 0.5f) / 0.5f;
        lineColor = { 255, (unsigned char)(255 - t * 255), 0, 255 };
    }

    // Línea de mira
    DrawLineEx({ startX, startY },
        { startX + dirX * lineLength, startY + dirY * lineLength },
        3.0f, lineColor);

    // Círculo en la punta de la mira
    DrawCircle((int)(startX + dirX * lineLength),
        (int)(startY + dirY * lineLength),
        5 + power * 10, lineColor);

    // Indicador de potencia en la mira
    DrawText(TextFormat("%d%%", (int)(power * 100)),
        (int)(startX + dirX * lineLength + 10),
        (int)(startY + dirY * lineLength - 10),
        16, WHITE);

    // Barra de potencia lateral
    float barX = 40.0f;
    float barY = 300.0f;
    float barWidth = 20.0f;
    float barHeight = 200.0f;

    DrawRectangle(barX, barY, barWidth, barHeight, Fade(WHITE, 0.3f));
    DrawRectangle(barX, barY + barHeight * (1.0f - power), barWidth, barHeight * power, lineColor);
    DrawRectangleLines(barX, barY, barWidth, barHeight, WHITE);
    DrawText("POT", barX - 5, barY - 25, 16, WHITE);
}

void Game::DrawUI() {
    // Panel superior
    DrawRectangle(90, 70, 820, 80, Fade(BLACK, 0.15f));

    DrawText("MAVI II - Unidad 4: Eventos y Colisiones", 120, 90, 28, RAYWHITE);

    const char* stateText = "";
    Color stateColor = WHITE;

    switch (currentState) {
    case GameState::Waiting:
        stateText = "ESPACIO: mantener para cargar";
        stateColor = GREEN;
        break;
    case GameState::Charging:
        stateText = "CARGANDO... Suelta ESPACIO para disparar";
        stateColor = YELLOW;
        break;
    case GameState::Running:
        stateText = "Proyectil en vuelo...";
        stateColor = YELLOW;
        break;
    case GameState::Hit:
        stateText = "IMPACTO! Presiona ESPACIO o R para reiniciar";
        stateColor = RED;
        break;
    case GameState::Finished:
        stateText = "Juego terminado. Presiona R para reiniciar";
        stateColor = ORANGE;
        break;
    }

    DrawText(stateText, 320, 125, 22, stateColor);

    //if (currentState == GameState::Hit) {
    //    DrawText("IMPACTO!", 400, 300, 60, RED);
    //    DrawText("Presiona ESPACIO o R para jugar de nuevo", 280, 370, 20, WHITE);
    //}

    DrawText("Box2D + Raylib | ContactListener + eventos", 350, 560, 16, Fade(WHITE, 0.4f));
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(Color{ 30, 30, 50, 255 });

    physicsWorld.Draw();
    DrawAim();
    DrawUI();

    EndDrawing();
}

void Game::Run() {
    while (running && !WindowShouldClose()) {
        HandleInput();
        Update();
        Draw();
    }
}