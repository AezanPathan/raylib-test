#include "raylib.h"

int main(void)
{
    InitWindow(1280, 720, "HD2D Experiment");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(
            "HD2D Experiment",
            20,
            20,
            30,
            WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}