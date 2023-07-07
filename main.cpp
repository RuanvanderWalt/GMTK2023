#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;

class Player{
private:
    Vector2 playerPos;
    float speed = 6.0f;
    int radius = 20;
public:
    Color color = RED;
    Vector2 getPlayerPos() {
        return playerPos;
    }


    Player(){
        playerPos = {(float)SCREENWIDTH/2, (float)SCREENHEIGHT/2 };
    }
    void movePlayer(){

        Vector2 tempVect = {(float)0, (float)0};

        if(IsKeyDown(KEY_W)){
            tempVect.y -= 1.0f;
        }
        if(IsKeyDown(KEY_S)){
            tempVect.y += 1.0f;
        }
        if(IsKeyDown(KEY_A)){
            tempVect.x -= 1.0f;
        }
        if(IsKeyDown(KEY_D)){
            tempVect.x += 1.0f;
        }

        tempVect = Vector2Normalize(tempVect);

        tempVect = Vector2Scale(tempVect, speed);

        tempVect = Vector2Add(playerPos, tempVect);

        if(tempVect.x + radius > SCREENWIDTH){
            tempVect.x = SCREENWIDTH - radius;
        }
        if(tempVect.x - radius < 0){
            tempVect.x = 0 + radius;
        }
        if(tempVect.y + radius > SCREENHEIGHT){
            tempVect.y = SCREENHEIGHT - radius;
        }
        if(tempVect.y - radius < 0){
            tempVect.y = 0 + radius;
        }

        playerPos = tempVect;

    }
    void drawPlayer(){
        DrawCircle(playerPos.x, playerPos.y, radius, color);
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
        cooldown =60;
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
    int attackTimerMax= 65;
    int attackTimer =0;
    int clock = 0;


    while (!WindowShouldClose())
    {

        clock++;

        if (clock%300 == 0){
            attackTimerMax--;
        }


        attackTimer++;

        if (attackTimer>attackTimerMax){
            attackTimer = 0;
            attacks.push_back(new attack());
            while (!CheckCollisionPointLine(player.getPlayerPos(),attacks.back()->start,attacks.back()->end,50)){
                delete attacks.back();
                attacks.pop_back();
                attacks.push_back(new attack());
            }

        }

        bool collied = false;

        for (int i=0;i<attacks.size();i++){
            attacks[i]->update();
            if (attacks[i]->cooldown < -30){
                attacks.erase(std::next(attacks.begin(), i));
                break;
            }
            if (!collied && attacks[i]->cooldown <0){
                collied = CheckCollisionPointLine(player.getPlayerPos(),attacks[i]->start,attacks[i]->end,50);
            }

        }

        if (collied){
            player.color = GREEN;
        }

        player.movePlayer();


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
