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
Texture2D knife ;

class Player{
private:
    Vector2 playerPos;
    float maxSpeed = 7.0f;
    Vector2 currSpeed = {0.0f, 0.0f};
    float acceleration = 30.0f/60;
    float coefFriction = 0.95f;
    int radius = 16;
    int nextDashCooldown = 0;
    bool dashNext = false;
    Texture2D move1;
    Texture2D move2;
    Texture2D move3;
    Texture2D move4;
    int animtimer = 0;
    int frame =0;
    bool firstSplat = true;

public:
    Color color = RED;
    Vector2 getPlayerPos() {
        return playerPos;
    }
    int getRadius(){
        return radius;
    }

    int totalElasped=0;
    void inWine(){
        coefFriction = 1.05f;
        if(firstSplat){
            currSpeed = Vector2Scale(currSpeed, 0.85f);
            firstSplat = false;
        }

    }

    void outWine(){
        coefFriction = 0.95f;
        firstSplat = true;
    }

    Player(){
        playerPos = {(float)SCREENWIDTH/2, (float)SCREENHEIGHT/2 };
        move1 = LoadTexture("assets/cheese_move1.png");
        move2 = LoadTexture("assets/cheese_move2.png");
        move3 = LoadTexture("assets/cheese_move3.png");
        move4 = LoadTexture("assets/cheese_move4.png");
    }

    int getDashCooldown(){
        return nextDashCooldown;
    }

    void resetPlayer(){
        playerPos = {(float)SCREENWIDTH/2, (float)SCREENHEIGHT/2 };
        currSpeed = {0.0f, 0.0f};
        nextDashCooldown = 0;
        dashNext = false;
    }

    void movePlayer(){

        //currSpeed = maxSpeed;

        //currSpeed = currSpeed + acceleration > maxSpeed ? maxSpeed : currSpeed + acceleration;

        Vector2 tempVect = {0.0f, 0.0f};

        if(nextDashCooldown == 0){
            if(IsKeyDown(KEY_SPACE)){
                dashNext = true;
            }
        }


        if(IsKeyDown(KEY_W)){
            tempVect.y = tempVect.y - (1.95f - coefFriction);
        }
        if(IsKeyDown(KEY_S)){
            tempVect.y = tempVect.y + (1.95f - coefFriction);
        }
        if(IsKeyDown(KEY_A)){
            tempVect.x = tempVect.x - (1.95f - coefFriction);
        }
        if(IsKeyDown(KEY_D) ){
            tempVect.x = tempVect.x + (1.95f - coefFriction);
        }

        if(IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && currSpeed.y < 0){
            tempVect.y += coefFriction;
        }
        if(IsKeyUp(KEY_S) && IsKeyUp(KEY_W) && currSpeed.y > 0){
            tempVect.y -= coefFriction;
        }
        if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D) && currSpeed.x < 0){
            tempVect.x += coefFriction;
        }
        if(IsKeyUp(KEY_D) && IsKeyUp(KEY_A) && currSpeed.x > 0){
            tempVect.x -= coefFriction;
        }

        tempVect = Vector2Normalize(tempVect);

        tempVect = Vector2Scale(tempVect, acceleration);
        currSpeed = Vector2Add(currSpeed, tempVect);

        if(Vector2Length(currSpeed) > maxSpeed){
            currSpeed = Vector2Normalize(currSpeed);
            currSpeed = Vector2Scale(currSpeed, maxSpeed);
        }

        Vector2 tempPos = Vector2Add(playerPos, currSpeed);

        if(dashNext){
            dashNext = false;
            tempPos = Vector2Add(Vector2Scale(Vector2Normalize(currSpeed), 20*maxSpeed), playerPos);
            nextDashCooldown = 180;
        }





        if(tempPos.x + radius > XUPPER){
            tempPos.x = XUPPER - radius;
            currSpeed.x = 0.0f;
        }
        if(tempPos.x - radius < XLOWER){
            tempPos.x = XLOWER + radius;
            currSpeed.x = 0.0f;
        }
        if(tempPos.y + radius > YUPPER){
            tempPos.y = YUPPER- radius;
            currSpeed.y = 0.0f;
        }
        if(tempPos.y - radius < YLOWER){
            tempPos.y = YLOWER + radius;
            currSpeed.y = 0.0f;
        }

        if(nextDashCooldown > 0){
            nextDashCooldown--;
        }

        playerPos = tempPos;

    }

    void update(int elaspedframes){
        animtimer +=elaspedframes;
        totalElasped+=elaspedframes;
    }

    void drawPlayer(){



        if (animtimer>2){
            animtimer=0;
            frame++;
        }

        if (frame>4){
            frame = 0;
        }

        if (totalElasped < 30*60){
            DrawTextureRec(move1,Rectangle {(float) (32*frame),0,32,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
        } else if (totalElasped <60*60){
            DrawTextureRec(move2,Rectangle {(float) (32*frame),0,32,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
        } else if (totalElasped<90*60){
            DrawTextureRec(move3,Rectangle {(float) (32*frame),0,32,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
        } else if (totalElasped<120*60) {
            DrawTextureRec(move4,Rectangle {(float) (32*frame),0,32,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
        }

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
        Color col;
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

//                if (std::rand()%2 == 0){
                    col = Color {223 ,214,202, (unsigned char) (100+150.0f* (15.0f-Vector2Distance(along,Vector2Add(along,tempNormal)))/15.0f)};
//                } else {
//                    col = Color {42 ,102,113, (unsigned char) (100+150.0f* (15.0f-Vector2Distance(along,Vector2Add(along,tempNormal)))/15.0f)};
//                }

                along = Vector2Add(along,dir);
                DrawRectangleV(Vector2Add(along,tempNormal),Vector2 {3,3},col);
            }


        } else {

            int flip = 1;
            Vector2 location;
            if (start.y<=end.y){
                location =start;
                if (start.x<end.x){
                    flip =-1;
                }

            } else {
                location =end;
                if (end.x<start.x){
                    flip =-1;
                }
            }


            float place = (float)abs(cooldown)/(float)30;
            float angle = Vector2Angle(Vector2 {0,0},Vector2Subtract(start,end))*RAD2DEG +90;
            if (angle>90){
                angle -= 180;
            }

            Vector2 dir = Vector2Rotate({0,1},angle*DEG2RAD);
            dir = Vector2Scale(dir, Vector2Distance(start,end)*place);

            location = Vector2Add(location,dir);


            if (place < 0.5){
                 col = WHITE;
            } else {
                 col = {255,255,255,(unsigned char) (60+195*(1-place))};
            }
            DrawTexturePro(knife,Rectangle {0,0,(float)(32*flip),192},Rectangle {location.x,location.y,32,192},Vector2 {16,0},angle,col);
            DrawLineEx(location,Vector2Subtract(location,dir),8,BLACK);

        }

    }

    void update(int elaspedframes){
        cooldown-=elaspedframes;
    }

private:

};


struct Splat {
    Vector2 pos;
    int radius;
    Splat(Vector2 p,int r){
        pos = p;
        radius = r;
    }
};

class WineSplater {
private:
public:
    int cooldown;
    std::vector<Splat*> splats;
    WineSplater(){
        int Count = GetRandomValue(3,10);
        for (int i =0;i<Count;i++){
            splats.push_back(new Splat(Vector2 {(float)GetRandomValue(200,600),(float)GetRandomValue(100,350)},
                                       GetRandomValue(10,100)));
        }
        cooldown = 180;
    }
    ~WineSplater() {
        for (int i =0;i<splats.size();i++){
            delete splats[i];
        }
        splats.clear();
    }
    void draw(){
        for (int i =0;i<splats.size();i++){
            DrawCircle(splats[i]->pos.x,splats[i]->pos.y,splats[i]->radius,PURPLE);
        }
    }
    void EffectPlayer(Player &player){
        bool shouldAffect = false;
        for (int i=0;i<splats.size();i++){
            if (CheckCollisionCircles(splats[i]->pos,splats[i]->radius,player.getPlayerPos(),player.getRadius())){
                shouldAffect = true;
                break;
            }
        }

        if (shouldAffect){
            player.color = GREEN;
            player.inWine();
        }

    }


    void update (int elaspedframes){
        cooldown-=elaspedframes;
    }
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

    knife = LoadTexture("assets/knife.png");
    Music Track1 = LoadMusicStream("assets/2023 game jam music v2.wav");

    PlayMusicStream(Track1);

    Player player = Player();

    SetTargetFPS(60);

    WineSplater* wineSplater = nullptr;
    std::vector<attack*> attacks;
    attacks.push_back(new attack());
    int attackTimerMax= 65;
    int attackTimer =0;
    int clock = 0;
    int high = 0;
    int wineCooldown = 0;

    bool menuflag = true;

    double time = GetTime();
    int elaspedframe = 1;

    RenderTexture2D renderTexture = LoadRenderTexture(SCREENWIDTH, SCREENHEIGHT);
    Texture2D background = LoadTexture("assets/Background_Plate_without_grain.png");


    while (!WindowShouldClose())
    {



            elaspedframe = 0;
            elaspedframe = (GetTime()-time)*60;
            time = GetTime();

            UpdateMusicStream(Track1);

            if (wineSplater == nullptr) {
                wineCooldown+=elaspedframe;
            }

            if (wineCooldown >= 600) {
                wineSplater = new WineSplater();
                wineCooldown = 0;
            }

            if (clock > high) {
                high = clock;
            }

            if (menuflag) {
                if (menu(clock, high)) {
                    for (int i = 0; i < attacks.size(); i++) {
                        delete attacks[i];
                    }
                    attacks.clear();
                    menuflag = false;
                    attackTimerMax = 65;
                    clock = 0;
                    player.totalElasped = 0;
                    player.resetPlayer();
                }
                continue;
            }


            clock+=elaspedframe;

            if (clock % 300 == 0) {
                attackTimerMax-=elaspedframe;
            }


            attackTimer+=elaspedframe;

            if (attackTimer > attackTimerMax) {
                attackTimer = 0;
                attacks.push_back(new attack());
                while (!CheckCollisionPointLine(player.getPlayerPos(), attacks.back()->start, attacks.back()->end,
                                                50)) {
                    delete attacks.back();
                    attacks.pop_back();
                    attacks.push_back(new attack());
                }

            }

            bool collied = false;

            for (int i = 0; i < attacks.size(); i++) {
                attacks[i]->update(elaspedframe);
                if (attacks[i]->cooldown < -30) {
                    attacks.erase(std::next(attacks.begin(), i));
                    break;
                }
                if (!collied && attacks[i]->cooldown < 0) {
                    collied = CheckCollisionPointLine(player.getPlayerPos(), attacks[i]->start, attacks[i]->end, 38);
                }

            }

            if (collied) {
                menuflag = true;
            }


            player.color = RED;
            player.outWine();
            if (wineSplater != nullptr) {
                if (wineSplater->cooldown < 0) {
                    delete wineSplater;
                    wineSplater = nullptr;
                }
                if (wineSplater != nullptr) {
                    wineSplater->update(elaspedframe);
                    wineSplater->EffectPlayer(player);
                }
            }

            for (int i =0;i<elaspedframe;i++) {
                player.movePlayer();
            }
            player.update(elaspedframe);



        BeginTextureMode(renderTexture);

            ClearBackground(RAYWHITE);

            DrawTextureRec(background,Rectangle{0,0,800,450,},Vector2 {0,0},WHITE);


//            DrawRectangleLines(80, 33, 640, 384, BLACK);
//            DrawRectangleLines(100, 53, 620, 364, RED);

            if (wineSplater != nullptr) {

                wineSplater->draw();
            }



            player.drawPlayer();



            for (int i = 0; i < attacks.size(); i++) {
                attacks[i]->draw();
            }

//            DrawText("Move the player using the WASD keys", 10, 10, 24, DARKGRAY);
//            DrawText(("Time: " + std::to_string(clock / 60)).c_str(), 10, 34, 24, DARKGRAY);
//            DrawText(("High: " + std::to_string(high / 60)).c_str(), 10, 56, 24, DARKGRAY);
//            DrawText(("Dash Cooldown: " + std::to_string((float)player.getDashCooldown()/60)).c_str(), 10, 94, 24, DARKGRAY);

            EndTextureMode();


            BeginDrawing();
                DrawTexturePro(renderTexture.texture,Rectangle{0,0,SCREENWIDTH,-SCREENHEIGHT},Rectangle{0,0,(float) GetScreenWidth(),(float) GetScreenHeight()},Vector2{0,0},0,WHITE);
            EndDrawing();



    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
