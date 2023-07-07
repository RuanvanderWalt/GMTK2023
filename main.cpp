#include <raylib.h>
#include <iostream>

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;

class Player{
private:
    Vector2 playerPos;

public:
    Player(){
        playerPos = {(float)SCREENWIDTH/2, (float)SCREENHEIGHT/2 };
    }
    void moveLeft(){
        if(playerPos.x - 4.0 >= 40){
            playerPos.x -= 4.0;
        }
    }
    void moveRight(){
        if(playerPos.x + 4.0 <= SCREENWIDTH - 40) {
            playerPos.x += 4.0;
        }
    }
    void moveUp(){
        if(playerPos.y - 4.0 >= 40){
            playerPos.y -= 4.0;
        }
    }
    void moveDown(){
        if(playerPos.y + 4.0 <= SCREENHEIGHT - 40){
            playerPos.y += 4.0;
        }
    }
    void drawPlayer(){
        DrawCircle(playerPos.x, playerPos.y, 40, RED);
    }
};

int main(void)
{

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

    Player player = Player();

    SetTargetFPS(60);


    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        if(IsKeyDown(KEY_W)){
            player.moveUp();
        }
        if(IsKeyDown(KEY_S)){
            player.moveDown();
        }
        if(IsKeyDown(KEY_A)){
            player.moveLeft();
        }
        if(IsKeyDown(KEY_D)){
            player.moveRight();
        }

        DrawText("Move the player using the WASD keys", 10, 10, 24, DARKGRAY);

        player.drawPlayer();

        EndDrawing();

    }


    CloseWindow();

    return 0;
}
