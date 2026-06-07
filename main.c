#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"

#include "raymath.h"

typedef struct
{

    Vector3 position;
    float speed;
    Color color;
} Player;

typedef struct
{
    Vector3 position;
    Color color;
    bool collected;
} Collectible;

typedef struct
{
    Vector3 position;
    Color color;
    bool hit;
} Obstacle;

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

    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
        player->position.y -= player->speed;
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

    // Collectible collectiblesPosition = {5.0f, 0.0f, 5.0f};
    int totalCollectibels = 10;
    int totalObstacles = 7;
    // Collectible collectible = {.position = {5.0f, 0.0f, 5.0f}, .color = GREEN};
    Collectible collectibles[10];

    Obstacle obstacles[7];

    for (int i = 0; i < totalCollectibels; i++)
    {
        collectibles[i].position = (Vector3){i * 5.0f, 0.0f, 0.0f};

        collectibles[i].color = GREEN;
        collectibles[i].collected = false;
    }

    for (int i = 0; i < totalObstacles; i++){
        obstacles[i].position = (Vector3){i * 3.0f , 0.0f , 0.0f };

          obstacles[i].hit = false;
          
        if ((i % 2) == 0)
        {
            obstacles[i].color = BLUE;
        }
        else{
            obstacles[i].color = RED;
        }
        
    }

    float cubeSize = 2.0f;
    int score = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        UpdatePlayer(&player);
        FollowPlayerCamera(&camera, &player);

        DrawCube(
            player.position,
            cubeSize,
            cubeSize,
            cubeSize,
            player.color);

        DrawGrid(10, 1.0f);

        for (int i = 0; i < totalCollectibels; i++)
        {
            if (!collectibles[i].collected &&
                Vector3Distance(
                    player.position,
                    collectibles[i].position) < 2.0f)
            {
                collectibles[i].collected = true;
                score++;
            }
        }

        for (int i = 0; i < totalCollectibels; i++)
        {
            if (!collectibles[i].collected)
            {
                DrawCube(
                    collectibles[i].position,
                    1.0f,
                    1.0f,
                    1.0f,
                    collectibles[i].color);
            }
        }

          for (int i = 0; i < totalObstacles; i++)
        {
            
                DrawCube(obstacles[i].position,
                    1.0f,
                    1.0f,
                    1.0f,
                    obstacles[i].color);

            if (! obstacles[i].hit && Vector3Distance( player.position,obstacles[i].position) < 2.0f)
            {
                if ( obstacles[i].color.r != player.color.r ||
    obstacles[i].color.g != player.color.g ||
    obstacles[i].color.b != player.color.b)
                {
                    obstacles[i].hit = true;
                    score--;
                }
                
            }

            
        }

        EndMode3D();

        if (score >= totalCollectibels)
        {
            DrawText(
                "YOU WIN!",
                300,
                200,
                40,
                GREEN);
        }
        

        GuiGroupBox(
            (Rectangle){5, 70, 220, 170},
            "Debug");

        if (GuiButton((Rectangle){10, 180, 60, 30}, "Red"))
        {
            player.color = RED;
        }

        if (GuiButton((Rectangle){80, 180, 60, 30}, "Blue"))
        {
            player.color = BLUE;
        }

        if (GuiButton((Rectangle){150, 180, 60, 30}, "Green"))
        {
            player.color = GREEN;
        }

        GuiSlider(
            (Rectangle){10, 120, 200, 20},
            "Slow",
            "Fast",
            &player.speed,
            0.01f,
            1.0f);

        GuiSlider(
            (Rectangle){10, 150, 200, 20},
            "Small",
            "Big",
            &cubeSize,
            0.5f,
            5.0f);

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

        DrawText(
            TextFormat("Score: %d", score),
            10,
            10,
            20,
            BLACK);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
