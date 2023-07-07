#include <raylib.h>
#include <vector>
#include <iostream>

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;

class Player{
private:
    Vector2 playerPos;
    float speed = 10.0f;
public:
    Color color = RED;
    Vector2 getPlayerPos() {
        return playerPos;
    }


    Player(){
        playerPos = {(float)SCREENWIDTH/2, (float)SCREENHEIGHT/2 };
    }
    void moveLeft(){
        if(playerPos.x - speed >= 40){
            playerPos.x -= speed;
        }
    }
    void moveRight(){
        if(playerPos.x + speed <= SCREENWIDTH - 40) {
            playerPos.x += speed;
        }
    }
    void moveUp(){
        if(playerPos.y - speed >= 40){
            playerPos.y -= speed;
        }
    }
    void moveDown(){
        if(playerPos.y + speed <= SCREENHEIGHT - 40){
            playerPos.y += speed;
        }
    }
    void drawPlayer(){
        DrawCircle(playerPos.x, playerPos.y, 40, color);
    }
};


class attack {
public:
    Vector2 start;
    Vector2 end;
    int cooldown ;
    attack(){
        start={(float)(std::rand()%800),(float)(std::rand()%450)};
        end={(float)(std::rand()%800),(float)(std::rand()%450)};
        cooldown =120;
    }

    void draw() {
        if (cooldown > 0){
            DrawLineEx(start,end,30,GRAY);
        } else {
            DrawLineEx(start,end,30,RED);
        }

    }

    void update(){
        cooldown--;
    }

private:

};





int main(void)
{

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

    Player player = Player();

    SetTargetFPS(60);

    std::vector<attack*> attacks;
    attacks.push_back(new attack());
    int attackTimerMax= 180;
    int attackTimer =0;


    while (!WindowShouldClose())
    {
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

        attackTimer++;

        if (attackTimer>attackTimerMax){
            attackTimer = 0;
            attacks.push_back(new attack());
            attackTimerMax--;
        }

        bool collied = false;

        for (int i=0;i<attacks.size();i++){
            attacks[i]->update();
            if (attacks[i]->cooldown < -30){
                attacks.erase(std::next(attacks.begin(), i));
                break;
            }
            if (!collied && attacks[i]->cooldown <0){
                collied = CheckCollisionPointLine(player.getPlayerPos(),attacks[i]->start,attacks[i]->end,70);
            }

        }

        if (collied){
            player.color = GREEN;
        }


        BeginDrawing();

        ClearBackground(RAYWHITE);



        DrawText("Move the player using the WASD keys", 10, 10, 24, DARKGRAY);

        player.drawPlayer();

        for (int i=0;i<attacks.size();i++){
            attacks[i]->draw();
        }


        EndDrawing();

    }


    CloseWindow();

    return 0;
}
