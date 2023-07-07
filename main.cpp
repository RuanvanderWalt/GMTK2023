#include <raylib.h>
#include <iostream>

int main(void)
{

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    SetTargetFPS(60);


    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(RAYWHITE);

             EndDrawing();

    }


    CloseWindow();

    return 0;
}
