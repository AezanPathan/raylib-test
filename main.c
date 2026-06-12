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
    float frontOffsetZ = 0.12f;

    float sideOffsetX = 0.0f;
    float sideOffsetY = 1.0f;
    float sideOffsetZ = 0.00f;

    float backOffsetX = 0.0f;
    float backOffsetY = 1.0f;
    float backOffsetZ = -0.12f;

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

        // Matrix frontTransform =
        //     MatrixTranslate(
        //         player.position.x + frontOffsetX,
        //         player.position.y + frontOffsetY,
        //         player.position.z + frontOffsetZ);

        // Matrix sideTransform =
        //     MatrixTranslate(
        //         player.position.x + sideOffsetX,
        //         player.position.y + sideOffsetY,
        //         player.position.z + sideOffsetZ);

        // Matrix backTransform =
        //     MatrixTranslate(
        //         player.position.x + backOffsetX,
        //         player.position.y + backOffsetY,
        //         player.position.z + backOffsetZ);

        Matrix frontTransform =
            MatrixMultiply(
                MatrixRotateX(DEG2RAD * 90.0f),
                MatrixTranslate(
                    player.position.x,
                    player.position.y + 1.0f,
                    player.position.z + shellDepth));

        Matrix backTransform =
            MatrixMultiply(
                MatrixRotateY(DEG2RAD * 180.0f),
                MatrixMultiply(
                    MatrixRotateX(DEG2RAD * 90.0f),
                    MatrixTranslate(
                        player.position.x,
                        player.position.y + 1.0f,
                        player.position.z - shellDepth)));

        Matrix leftTransform =
            MatrixMultiply(
                MatrixRotateY(DEG2RAD * 90.0f),
                MatrixMultiply(
                    MatrixRotateX(DEG2RAD * 90.0f),
                    MatrixTranslate(
                        player.position.x - shellDepth,
                        player.position.y + 1.0f,
                        player.position.z)));

        Matrix rightTransform =
            MatrixMultiply(
                MatrixRotateY(DEG2RAD * -90.0f),
                MatrixMultiply(
                    MatrixRotateX(DEG2RAD * 90.0f),
                    MatrixTranslate(
                        player.position.x + shellDepth,
                        player.position.y + 1.0f,
                        player.position.z)));
        Matrix frontRotation = MatrixRotateX(DEG2RAD * 90.0f);
        Matrix sideRotation = MatrixMultiply(MatrixRotateY(billboardYaw), MatrixRotateX(DEG2RAD * 90.0f));
        Matrix backRotation = MatrixRotateX(DEG2RAD * 90.0f);

        frontTransform = MatrixMultiply(frontRotation, frontTransform);
      //  sideTransform = MatrixMultiply(sideRotation, sideTransform);
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

        // //  if (angleDegrees < 120.0f)
        // DrawMesh(quadMesh, frontMat, frontTransform);

        // //   if (angleDegrees > 60.0f && angleDegrees < 300.0f)
        // DrawMesh(quadMesh, sideMat, sideTransform);

        // //  if (angleDegrees > 180.0f)
        // DrawMesh(quadMesh, backMat, backTransform);


DrawMesh(quadMesh, frontMat, frontTransform);
DrawMesh(quadMesh, backMat, backTransform);
DrawMesh(quadMesh, leftMat, leftTransform);
DrawMesh(quadMesh, rightMat, rightTransform);

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

/*

    // front control
        DrawText("Front X", 15, 100, 10, BLACK);
        GuiSlider(
            (Rectangle){80, 100, 200, 20},
            "",
            "",
            &frontOffsetX,
            -1.0f,
            1.0f);

        DrawText("Front Y", 15, 130, 10, BLACK);
        GuiSlider(
            (Rectangle){80, 130, 200, 20},
            "",
            "",
            &frontOffsetY,
            0.0f,
            3.0f);

        DrawText("Front Z", 15, 160, 10, BLACK);
        GuiSlider(
            (Rectangle){80, 160, 200, 20},
            "",
            "",
            &frontOffsetZ,
            -1.0f,
            1.0f);
        // side controle

        DrawText("Side X", 15, 210, 10, BLACK);
        GuiSlider(
            (Rectangle){80, 210, 200, 20},
            "",
            "",
            &sideOffsetX,
            -1.0f,
            1.0f);

        DrawText("Side Y", 15, 240, 10, BLACK);
        GuiSlider(
            (Rectangle){80, 240, 200, 20},
            "",
            "",
            &sideOffsetY,
            0.0f,
            3.0f);

        DrawText("Side Z", 15, 270, 10, BLACK);
        GuiSlider(
            (Rectangle){80, 270, 200, 20},
            "",
            "",
            &sideOffsetZ,
            -1.0f,
            1.0f);
        // back controle

        DrawText("Back X", 15, 320, 10, BLACK);
        GuiSlider(
            (Rectangle){80, 320, 200, 20},
            "",
            "",
            &backOffsetX,
            -1.0f,
            1.0f);

        DrawText("Back Y", 15, 350, 10, BLACK);
        GuiSlider(
            (Rectangle){80, 350, 200, 20},
            "",
            "",
            &backOffsetY,
            0.0f,
            3.0f);

        DrawText("Back Z", 15, 380, 10, BLACK);

        GuiSlider(
            (Rectangle){80, 380, 200, 20},
            "",
            "",
            &backOffsetZ,
            -1.0f,
            1.0f);

        /// rotation

        // front
        GuiSlider(
            (Rectangle){80, 180, 200, 20},
            "",
            "",
            &frontRotX,
            -180,
            180);

        GuiSlider(
            (Rectangle){80, 210, 200, 20},
            "",
            "",
            &frontRotY,
            -180,
            180);

        GuiSlider(
            (Rectangle){80, 240, 200, 20},
            "",
            "",
            &frontRotZ,
            -180,
            180);

        // side

        GuiSlider(
            (Rectangle){80, 300, 200, 20},
            "",
            "",
            &sideRotX,
            -180,
            180);

        GuiSlider(
            (Rectangle){80, 330, 200, 20},
            "",
            "",
            &sideRotY,
            -180,
            180);

        GuiSlider(
            (Rectangle){80, 360, 200, 20},
            "",
            "",
            &sideRotZ,
            -180,
            180);

        // back

        GuiSlider(
            (Rectangle){80, 420, 200, 20},
            "",
            "",
            &backRotX,
            -180,
            180);

        GuiSlider(
            (Rectangle){80, 450, 200, 20},
            "",
            "",
            &backRotY,
            -180,
            180);

        GuiSlider(
            (Rectangle){80, 480, 200, 20},
            "",
            "",
            &backRotZ,
            -180,
            180);

        // draw current value

        DrawText(
            TextFormat("F %.2f %.2f %.2f",
                       frontOffsetX,
                       frontOffsetY,
                       frontOffsetZ),
            15,
            420,
            10,
            BLACK);

        DrawText(
            TextFormat("S %.2f %.2f %.2f",
                       sideOffsetX,
                       sideOffsetY,
                       sideOffsetZ),
            15,
            435,
            10,
            BLACK);

        DrawText(
            TextFormat("B %.2f %.2f %.2f",
                       backOffsetX,
                       backOffsetY,
                       backOffsetZ),
            15,
            450,
            10,
            BLACK);


*/