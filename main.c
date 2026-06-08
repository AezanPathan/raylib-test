#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

typedef struct
{
    Vector3 position;
    float speed;
} Player;

// --------------------
// Player
// --------------------

void UpdatePlayer(Player *player)
{
    if (IsKeyDown(KEY_D))
        player->position.x += player->speed;

    if (IsKeyDown(KEY_A))
        player->position.x -= player->speed;

    if (IsKeyDown(KEY_W))
        player->position.z += player->speed;

    if (IsKeyDown(KEY_S))
        player->position.z -= player->speed;

    if (IsKeyDown(KEY_SPACE))
        player->position.y += player->speed;
}

// --------------------
// Camera
// --------------------

void FollowPlayerCamera(Camera3D *camera, Player *player)
{
    camera->position.x = player->position.x + 4.0f;
    camera->position.y = player->position.y + 4.0f;
    camera->position.z = player->position.z + 4.0f;

    camera->target = player->position;
}

// --------------------
// Main
// --------------------

int main(void)
{
    InitWindow(1280, 720, "HD2D Experiment");

    float cameraAngle = 0.0f;

    Camera3D camera = {0};
    camera.position = (Vector3){4, 4, 4};
    camera.target = (Vector3){0, 0, 0};
    camera.up = (Vector3){0, 1, 0};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    Player player = {
        .position = {0, 0, 0},
        .speed = 0.1f};

    Texture2D hero = LoadTexture("assets/16x32 Rotate-Sheet.png");

    int direction = 0;

    while (!WindowShouldClose())
    {
        // Update
        UpdatePlayer(&player);
        //  FollowPlayerCamera(&camera, &player);

        if (IsKeyPressed(KEY_KP_1))
            direction = 0;
        if (IsKeyPressed(KEY_KP_2))
            direction = 1;
        if (IsKeyPressed(KEY_KP_3))
            direction = 2;
        if (IsKeyPressed(KEY_KP_4))
            direction = 3;
        if (IsKeyPressed(KEY_KP_5))
            direction = 4;
        if (IsKeyPressed(KEY_KP_6))
            direction = 5;
        if (IsKeyPressed(KEY_KP_7))
            direction = 6;
        if (IsKeyPressed(KEY_KP_8))
            direction = 7;

        if (IsKeyDown(KEY_Q))
            cameraAngle -= 0.02f;

        if (IsKeyDown(KEY_E))
            cameraAngle += 0.02f;

        float mouseDelta =
            GetMouseDelta().x;

        cameraAngle += mouseDelta * 0.01f;

        camera.position.x =
            player.position.x + cosf(cameraAngle) * 6.0f;

        camera.position.z =
            player.position.z + sinf(cameraAngle) * 6.0f;

        camera.position.y =
            player.position.y + 4.0f;

        camera.target = player.position;

        // Draw
        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode3D(camera);

        DrawGrid(10, 1.0f);

        Rectangle source = {
            direction * 32,
            0,
            32,
            32};

        DrawBillboardRec(
            camera,
            hero,
            source,
            player.position,
            (Vector2){2.0f, 4.0f},
            WHITE);

        EndMode3D();

        GuiGroupBox(
            (Rectangle){5, 70, 240, 170},
            "Debug UI");

        GuiSlider(
            (Rectangle){10, 120, 200, 20},
            "Slow",
            "Fast",
            &player.speed,
            0.01f,
            1.0f);

        EndDrawing();
    }

    UnloadTexture(hero);
    CloseWindow();

    return 0;
}