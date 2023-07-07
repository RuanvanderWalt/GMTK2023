#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
const int XUPPER = 720;
const int XLOWER = 80;
const int YUPPER = 417;
const int YLOWER = 33;

class Player{
private:
    Vector2 playerPos;
    float speed = 8.0f;
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

        if(tempVect.x + radius > XUPPER){
            tempVect.x = XUPPER - radius;
        }
        if(tempVect.x - radius < XLOWER){
            tempVect.x = XLOWER + radius;
        }
        if(tempVect.y + radius > YUPPER){
            tempVect.y = YUPPER- radius;
        }
        if(tempVect.y - radius < YLOWER){
            tempVect.y = YLOWER + radius;
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
//            DrawLineEx(start,end,30,GRAY);
            int Distance = Vector2Distance(start,end);
            Vector2 dir  = Vector2Normalize(Vector2Subtract(end,start));
            Vector2 along = start;
            Vector2 Normal = Vector2Rotate(dir,90.0*DEG2RAD);
            Vector2 tempNormal;
            Vector2 Middle = Vector2Subtract(end,start);
            Middle = Vector2Add(start, Vector2Scale(Middle,0.5));
            for (int i=0;i<Distance;i++)
            {
                tempNormal = Normal;
                tempNormal = Vector2Scale(tempNormal,-15+ std::rand()%30);
                tempNormal = Vector2Scale(tempNormal,((Distance/2)- Vector2Distance(Middle,along))/(Distance/2));

                along = Vector2Add(along,dir);
                DrawRectangleV(Vector2Add(along,tempNormal),Vector2 {5,5},Color {0 ,100,125,(unsigned char) (200.0f* (15.0f-Vector2Distance(along,Vector2Add(along,tempNormal)))/15.0f)});
            }
            DrawRectangleV(Middle,Vector2 {15,15},GREEN);

        } else {
            DrawLineEx(start,end,30,RED);
        }

    }

    void update(){
        cooldown--;
    }

private:

};


bool menu(int& clock,int& high){
    bool out = false;



    if (IsKeyPressed(KEY_ENTER)){
        out =true;
    }

    BeginDrawing();
    ClearBackground(SKYBLUE);


    if (high != 0){
        DrawText(("High Score: "+std::to_string(high/60)).c_str(), 200, 34, 24, ORANGE);
    }

    EndDrawing();

    return out;
}


int main(){

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "raylib [core] example - keyboard input");

    InitAudioDevice();

    Music Track1 = LoadMusicStream("assets/2023 game jam music 1.wav");

    PlayMusicStream(Track1);

    Player player = Player();

    SetTargetFPS(60);

    std::vector<attack*> attacks;
    attacks.push_back(new attack());
    int attackTimerMax= 65;
    int attackTimer =0;
    int clock = 0;
    int high = 0;

    bool menuflag = true;


    while (!WindowShouldClose())
    {

        UpdateMusicStream(Track1);

        if (clock>high){
            high=clock;
        }

        if (menuflag){
            if (menu(clock,high)){
                for(int i =0;i<attacks.size();i++){
                    delete attacks[i];
                }
                attacks.clear();
                menuflag = false;
                attackTimerMax = 65;
                clock =0;
            }
            continue;
        }



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
            menuflag = true;
        }

        player.movePlayer();


        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangleLines(80, 33, 640, 384, BLACK);
        DrawRectangleLines(100, 53, 620, 364, RED);



        player.drawPlayer();

        for (int i=0;i<attacks.size();i++){
            attacks[i]->draw();
        }

        DrawText("Move the player using the WASD keys", 10, 10, 24, DARKGRAY);
        DrawText(("Time: "+std::to_string(clock/60)).c_str(), 10, 34, 24, DARKGRAY);
        DrawText(("High: "+std::to_string(high/60)).c_str(), 10, 56, 24, DARKGRAY);


        EndDrawing();

    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
