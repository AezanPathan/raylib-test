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

    Mesh quadMesh = GenMeshPlane(1.0f, 2.0f, 1, 1);

    Material material = LoadMaterialDefault();
    material.maps[MATERIAL_MAP_DIFFUSE].texture = hero;

    frontMat.maps[MATERIAL_MAP_DIFFUSE].texture = front;
    frontMat.maps[MATERIAL_MAP_DIFFUSE].color = WHITE;

    frontMat.shader = LoadShader(0, 0);
    int direction = 0;

    SetTargetFPS(60);

    Rectangle frontFrame = {0, 0, 32, 32};
    Rectangle sideFrame = {64, 0, 32, 32};
    Rectangle backFrame = {128, 0, 32, 32};

    // Discard plan background for the sprites with the custom shader
    Shader alphaDiscard = LoadShader("shaders/alpha_discard.vs", "shaders/alpha_discard.fs");

    frontMat.shader = alphaDiscard;
    sideMat.shader = alphaDiscard;
    backMat.shader = alphaDiscard;

    Material leftMat = LoadMaterialDefault();
    leftMat.maps[MATERIAL_MAP_DIFFUSE].texture = side;

    Material rightMat = LoadMaterialDefault();
    rightMat.maps[MATERIAL_MAP_DIFFUSE].texture = side;

    leftMat.shader = alphaDiscard;
    rightMat.shader = alphaDiscard;

    float shellDepth = 0.15f; // define a shell thickness

    // for the debug purpose
    float frontOffsetX = 0.0f;
    float frontOffsetY = 1.0f;
    float frontOffsetZ = 0.15f;

    float sideOffsetX = 0.0f;
    float sideOffsetY = 1.0f;
    float sideOffsetZ = 0.00f;

    float backOffsetX = 0.0f;
    float backOffsetY = 1.0f;
    float backOffsetZ = -0.15f;

    // rotation
    float frontRotX = 90.0f;
    float frontRotY = 0.0f;
    float frontRotZ = 0.0f;

    float sideRotX = 90.0f;
    float sideRotY = 0.0f;
    float sideRotZ = 0.0f;

    float backRotX = 90.0f;
    float backRotY = 0.0f;
    float backRotZ = 0.0f;

    while (!WindowShouldClose())
    {
        // Update
        UpdatePlayer(&player);

        if (IsKeyDown(KEY_Q))
            cameraAngle -= 0.02f;

        if (IsKeyDown(KEY_E))
            cameraAngle += 0.02f;

        direction = GetDirectionFromAngle(cameraAngle, 8);

        float mouseDelta = GetMouseDelta().x;

        cameraAngle += mouseDelta * 0.003f;

        camera.position.x = player.position.x + cosf(cameraAngle) * 6.0f;

        camera.position.z = player.position.z + sinf(cameraAngle) * 6.0f;

        camera.position.y = player.position.y + 4.0f;

        camera.target = player.position;

        float billboardYaw = atan2f(
            camera.position.x - player.position.x,
            camera.position.z - player.position.z);

        Matrix frontTransform =
            MatrixTranslate(
                player.position.x + frontOffsetX,
                player.position.y + frontOffsetY,
                player.position.z + frontOffsetZ);

        Matrix sideTransform =
            MatrixTranslate(
                player.position.x + sideOffsetX,
                player.position.y + sideOffsetY,
                player.position.z + sideOffsetZ);

        Matrix backTransform =
            MatrixTranslate(
                player.position.x + backOffsetX,
                player.position.y + backOffsetY,
                player.position.z + backOffsetZ);
        Matrix frontRotation = MatrixRotateX(DEG2RAD * 90.0f);
       // Matrix sideRotation = MatrixMultiply(MatrixRotateY(billboardYaw), MatrixRotateX(DEG2RAD * 90.0f));
        Matrix backRotation = MatrixRotateX(DEG2RAD * 90.0f);

     Matrix sideRotation = MatrixMultiply(MatrixRotateZ(billboardYaw), MatrixRotateX(DEG2RAD * 90.0f));


         

        frontTransform = MatrixMultiply(frontRotation, frontTransform);
        sideTransform = MatrixMultiply(sideRotation, sideTransform);
        backTransform = MatrixMultiply(backRotation, backTransform);

        // Draw
        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode3D(camera);

        DrawPlane((Vector3){0, 0, 0}, (Vector2){20, 20}, DARKGRAY);

        //     DrawCube(
        // (Vector3){0,1,0},
        // 1.0f,
        // 2.0f,
        // 1.0f,
        // RED);

        Rectangle source = {
            direction * 32,
            0,
            32,
            32};

        Vector3 positions[3] = {
            {0, 0, 0},
            {0.3f, 0, 0},
            {-0.3f, 0, 0}};

        float angleDegrees = cameraAngle * RAD2DEG;

        while (angleDegrees < 0)
            angleDegrees += 360.0f;

        while (angleDegrees >= 360.0f)
            angleDegrees -= 360.0f;

        rlDisableBackfaceCulling();

        if (angleDegrees < 120.0f)
        DrawMesh(quadMesh, frontMat, frontTransform);

         if (angleDegrees > 60.0f && angleDegrees < 300.0f)
        DrawMesh(quadMesh, sideMat, sideTransform);

        if (angleDegrees > 180.0f)
        DrawMesh(quadMesh, backMat, backTransform);


        rlEnableBackfaceCulling();
        EndMode3D();

        GuiGroupBox(
            (Rectangle){5, 70, 350, 520},
            "Sprite Debug");

        // GuiSlider(
        //     (Rectangle){10, 120, 200, 20},
        //     "Slow",
        //     "Fast",
        //     &player.speed,
        //     0.01f,
        //     1.0f);

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
