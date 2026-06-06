#include "raylib.h"

typedef struct
{

    Vector3 position;
    float speed;
    Color color;
} Player;

void UpdatePlayer(Player *player)
{
    if (IsKeyDown(KEY_D))
      player->position.x += player->speed;

    if (IsKeyDown(KEY_A))
       player->position.x -= player->speed;

    if (IsKeyDown(KEY_W))
        player->position.z +=  player->speed;

    if (IsKeyDown(KEY_S))
         player->position.z -=  player->speed;

    if (IsKeyDown(KEY_SPACE))
        player->position.y +=  player->speed;

    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
       player->position.y -=  player->speed;
}

void FollowPlayerCamera(Camera3D *camera, Player *player)
{
    camera->position.x = player->position.x + 4.0f;
    camera->position.y = player->position.y + 4.0f;
    camera->position.z = player->position.z + 4.0f;

    camera->target = player->position;
}


int main(void)
{
    InitWindow(800, 450, "Hello Raylib!");
    Camera3D camera = {0};
    camera.position = (Vector3){4.0f, 4.0f, 4.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Player player = {
        .position = {0.0f, 0.0f, 0.0f},
        .speed = 0.1f,
        .color = RED};
   // Vector3 cubePosition = {0.0f, 0.0f, 0.0f};
  
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        FollowPlayerCamera(&camera , &player);
        UpdatePlayer(&player);

        DrawCube(
            player.position,
            2.0f,
            2.0f,
            2.0f,
            player.color);

        DrawGrid(10, 1.0f);
        EndMode3D();

        DrawFPS(10, 10);

        DrawText(
            TextFormat(
                "Pos: %.2f %.2f %.2f",
                player.position.x,
                player.position.y,
                player.position.z),
            10,
            40,
            20,
            BLUE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
