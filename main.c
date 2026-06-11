#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"
#include "raymath.h"
#include "rlgl.h"

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

int GetDirectionFromAngle(float angleRadians, int totalDirections)
{
    float angleDegrees = angleRadians * RAD2DEG;

    while (angleDegrees < 0)
        angleDegrees += 360.0f;

    while (angleDegrees >= 360.0f)
        angleDegrees -= 360.0f;

    float sliceSize = 360.0f / totalDirections;

    return (int)((angleDegrees + sliceSize / 2.0f) / sliceSize) % totalDirections;
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

    Texture2D front = LoadTexture("assets/front.png");
    Texture2D side = LoadTexture("assets/side.png");
    Texture2D back = LoadTexture("assets/back.png");

    Material frontMat = LoadMaterialDefault();
    frontMat.maps[MATERIAL_MAP_DIFFUSE].texture = front;

    Material sideMat = LoadMaterialDefault();
    sideMat.maps[MATERIAL_MAP_DIFFUSE].texture = side;

    Material backMat = LoadMaterialDefault();
    backMat.maps[MATERIAL_MAP_DIFFUSE].texture = back;

    // Mesh quadMesh = GenMeshPlane(2.0f, 4.0f, 1, 1);
    Mesh quadMesh = GenMeshPlane(1.0f, 2.0f, 1, 1);

    Material material = LoadMaterialDefault();
    material.maps[MATERIAL_MAP_DIFFUSE].texture = hero;

    int direction = 0;

    SetTargetFPS(60);

    Rectangle frontFrame = {0, 0, 32, 32};
    Rectangle sideFrame = {64, 0, 32, 32};
    Rectangle backFrame = {128, 0, 32, 32};

   Matrix frontTransform =
    MatrixMultiply(
        MatrixRotateX(DEG2RAD * 90.0f),
        MatrixTranslate(0.0f, 2.0f, 0.10f));

Matrix sideTransform =
    MatrixMultiply(
        MatrixRotateY(DEG2RAD * 90.0f),
        MatrixMultiply(
            MatrixRotateX(DEG2RAD * 90.0f),
            MatrixTranslate(0.10f, 2.0f, 0.0f)));

Matrix backTransform =
    MatrixMultiply(
        MatrixRotateY(DEG2RAD * 180.0f),
        MatrixMultiply(
            MatrixRotateX(DEG2RAD * 90.0f),
            MatrixTranslate(0.0f, 2.0f, -0.10f)));

    while (!WindowShouldClose())
    {
        // Update
        UpdatePlayer(&player);

        direction = GetDirectionFromAngle(cameraAngle, 8);

        float mouseDelta =
            GetMouseDelta().x;

        cameraAngle += mouseDelta * 0.003f;

        camera.position.x = player.position.x + cosf(cameraAngle) * 6.0f;

        camera.position.z = player.position.z + sinf(cameraAngle) * 6.0f;

        camera.position.y = player.position.y + 4.0f;

        camera.target = player.position;

        // Draw
        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode3D(camera);

        DrawPlane((Vector3){0, 0, 0}, (Vector2){20, 20}, DARKGRAY);

        Rectangle source = {
            direction * 32,
            0,
            32,
            32};

        Vector3 positions[3] = {
            {0, 0, 0},
            {0.3f, 0, 0},
            {-0.3f, 0, 0}};

        // DrawBillboardRec(camera, hero, frontFrame, positions[0], (Vector2){2, 4}, WHITE);

        //  DrawBillboardRec(camera, hero, sideFrame, positions[1], (Vector2){2, 4}, WHITE);

        //  DrawBillboardRec(camera, hero, backFrame, positions[2], (Vector2){2, 4}, WHITE);

        float angleDegrees = cameraAngle * RAD2DEG;

        while (angleDegrees < 0)
            angleDegrees += 360.0f;

        while (angleDegrees >= 360.0f)
            angleDegrees -= 360.0f;

            rlDisableBackfaceCulling();

      //  if (angleDegrees < 120.0f)
            DrawMesh(quadMesh, frontMat, frontTransform);

       // if (angleDegrees > 60.0f && angleDegrees < 300.0f)
           DrawMesh(quadMesh, sideMat, sideTransform);

      //  if (angleDegrees > 180.0f)
          DrawMesh(quadMesh, backMat, backTransform);

          rlEnableBackfaceCulling();
        //   DrawCube(
        // (Vector3){0,2,0},
        // 1.0f,
        // 2.0f,
        // 0.1f,
        // RED);

        
        
           // DrawMesh(quadMesh, backMat, transform3);

        //       DrawCube(
        // (Vector3){0,2,0},
        // 1.0f,
        // 2.0f,
        // 0.1f,
        // PURPLE);
        // DrawCube(
        //     (Vector3){0,2,0},
        //     1.0f,
        //     2.0f,
        //     0.1f,
        //     PURPLE);

        //   DrawMesh(quadMesh, material, MatrixTranslate(player.position.x, player.position.y + 2.0f, player.position.z));
        // DrawBillboardRec(
        //     camera,
        //     hero,
        //     source,
        //     player.position,
        //     (Vector2){2.0f, 4.0f},
        //     WHITE);

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

        // DrawText(
        //     TextFormat(
        //         "Direction: %d",
        //         direction),
        //     10,
        //     35,
        //     20,
        //   BLACK);
        DrawText(
            TextFormat("Angle Degrees: %.2f", angleDegrees),
            20,
            35,
            20,
            BLACK);

        EndDrawing();
    }

    UnloadTexture(hero);
    CloseWindow();

    return 0;
}