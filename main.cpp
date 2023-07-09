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
Sound cut;
Sound splat;
Sound cheese;
Sound dash;
Texture2D wineBig;
Texture2D wineMedium;
Texture2D wineSmall;
Texture2D wineLarge;
Texture2D waring;
Texture2D dashTexture;
Texture2D tutorial;
Texture2D wine;
Texture2D knifeFlat;
Texture2D tutorial2;
Texture2D mainMenu;
Texture2D difficulties;

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
    Texture2D moveDash;
    int animtimer = 0;
    int frame =0;
    bool firstSplat = true;
    int dashCount = 0;


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
        moveDash = LoadTexture("assets/cheese_dash.png");
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

        Vector2 tempPos = {0.0f, 0.0f};

        if(dashCount > 0){
            dashCount--;
            tempPos = Vector2Add(Vector2Scale(Vector2Normalize(currSpeed), 4*maxSpeed), playerPos);

        }
        else{
            Vector2 tempVect = {0.0f, 0.0f};

            if(nextDashCooldown == 0){
                if(IsKeyDown(KEY_SPACE)){
                    dashNext = true;
                    PlaySound(dash);
                }
                if (IsGamepadAvailable(0)){
                    if (IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_FACE_DOWN)){
                        dashNext = true;
                        PlaySound(dash);
                    }
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




            Vector2 tempAcc = tempVect;

            if(IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && currSpeed.y < 0){
                tempAcc.y += coefFriction;
            }
            if(IsKeyUp(KEY_S) && IsKeyUp(KEY_W) && currSpeed.y > 0){
                tempAcc.y -= coefFriction;
            }
            if(IsKeyUp(KEY_A) && IsKeyUp(KEY_D) && currSpeed.x < 0){
                tempAcc.x += coefFriction;
            }
            if(IsKeyUp(KEY_D) && IsKeyUp(KEY_A) && currSpeed.x > 0){
                tempAcc.x -= coefFriction;
            }

            if (IsGamepadAvailable(0)){
                Vector2 gamepadAxis {GetGamepadAxisMovement(0,0),GetGamepadAxisMovement(0,1)};
                if (Vector2Length(gamepadAxis) !=0){
                    tempAcc = Vector2Scale(gamepadAxis,(1.95f - coefFriction));
                    tempVect =tempAcc;
                }
            }

            tempAcc = Vector2Normalize(tempAcc);

            tempAcc = Vector2Scale(tempAcc, acceleration);
            currSpeed = Vector2Add(currSpeed, tempAcc);

            if(Vector2Length(currSpeed) > maxSpeed){
                currSpeed = Vector2Normalize(currSpeed);
                currSpeed = Vector2Scale(currSpeed, maxSpeed);
            }

            tempPos = Vector2Add(playerPos, currSpeed);

            if(dashNext){
                dashNext = false;
                dashCount = 5;
                currSpeed = Vector2Normalize(tempVect);
                tempPos = Vector2Add(Vector2Scale(currSpeed, 4*maxSpeed), playerPos);
                nextDashCooldown = 150;
            }
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

        float flip = 1;

        if (currSpeed.x < 0 && abs(currSpeed.x)>2){
            flip = -1;
        }

        if (animtimer>2){
            animtimer=0;
            frame++;
        }




        if (frame>4){
            frame = 0;

        }


        if(dashCount > 0){
            float tempAngle = Vector2Angle({0,0}, currSpeed)*RAD2DEG;
            tempAngle = tempAngle < -90 ? 180 + tempAngle : tempAngle;
            tempAngle = tempAngle > 90 ? 180 - tempAngle : tempAngle;
            //DrawText(("Dash Angle: " + std::to_string((float)tempAngle)).c_str(), 10, 70, 24, WHITE);
            DrawTexturePro(moveDash, Rectangle {0, 0, 32, 32}, Rectangle {playerPos.x, playerPos.y, 32, 32}, {16,16},
                           tempAngle , WHITE);
        }
        else{
            if (totalElasped < 30*60){
                DrawTextureRec(move1,Rectangle {(float) (32*frame),0,32*flip,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
            } else if (totalElasped <60*60){
                DrawTextureRec(move2,Rectangle {(float) (32*frame),0,32*flip,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
            } else if (totalElasped<90*60){
                DrawTextureRec(move3,Rectangle {(float) (32*frame),0,32*flip,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
            } else if (totalElasped<120*60) {
                DrawTextureRec(move4,Rectangle {(float) (32*frame),0,32*flip,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
            } else {
                DrawTextureRec(move4,Rectangle {(float) (32*frame),0,32*flip,32}, Vector2Subtract(playerPos,Vector2{(float )radius+2,(float)radius+2}),WHITE);
            }

            if (nextDashCooldown<=0){

                float coef= sin(((float)totalElasped*DEG2RAD)*3.5)*0.5f;
                coef = abs(coef);


                DrawTexturePro(dashTexture, Rectangle {0, 0, 32, 32}, Rectangle {playerPos.x - 8 - 8 * coef, playerPos.y - 30 - 8 * coef, 16 + 8 * coef, 16 + 8 * coef}, Vector2 {0, 0}, 0, WHITE);
            }


        }

    }
};


class attack {
public:
    Vector2 start;
    Vector2 end;
    int cooldown ;
    bool hasPlayed = false;

    attack(){
        start={(float)(GetRandomValue(XLOWER,XUPPER)),(float)(GetRandomValue(YLOWER,YUPPER))};
        end={(float)(GetRandomValue(XLOWER,XUPPER)),(float)(GetRandomValue(YLOWER,YUPPER))};
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
                along = Vector2Add(along,dir);
                if (i%16 ==0){
                    if (i==0){
                        DrawTexturePro(waring, Rectangle {0,32,16,16},Rectangle {along.x,along.y,16,16},Vector2 {16,16},
                                       Vector2Angle(Vector2 {0,0},Normal)*RAD2DEG,Color {150,150,150,150});
                    } else if (i>Distance-16){
                        DrawTexturePro(waring, Rectangle {0,0,16,16},Rectangle {along.x,along.y,16,16},Vector2 {16,16},
                                       Vector2Angle(Vector2 {0,0},Normal)*RAD2DEG,Color {150,150,150,150});
                    } else {
                        DrawTexturePro(waring, Rectangle {0,16,16,16},Rectangle {along.x,along.y,16,16},Vector2 {16,16},
                                       Vector2Angle(Vector2 {0,0},Normal)*RAD2DEG,Color {150,150,150,150});
                    }
                }

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

            int Distance = Vector2Distance(location,Vector2Subtract(location,dir));
            Vector2 along = Vector2Subtract(location,dir);
            dir = Vector2Normalize(dir);
            Vector2 Normal = Vector2Rotate(dir,90.0*DEG2RAD);

            for (int i=0;i<Distance;i++)
            {
                along = Vector2Add(along,dir);
                if (i%16 ==0){
                    if (i==0){
                        DrawTexturePro(waring, Rectangle {0,32,16,16},Rectangle {along.x,along.y,16,16},Vector2 {16,16},
                                       Vector2Angle(Vector2 {0,0},Normal)*RAD2DEG,Color {255,255,255,255});
                    } else if (i>Distance-16){
                        DrawTexturePro(waring, Rectangle {0,0,16,16},Rectangle {along.x,along.y,16,16},Vector2 {16,16},
                                       Vector2Angle(Vector2 {0,0},Normal)*RAD2DEG,Color {255,255,255,255});
                    } else {
                        DrawTexturePro(waring, Rectangle {0,16,16,16},Rectangle {along.x,along.y,16,16},Vector2 {16,16},
                                       Vector2Angle(Vector2 {0,0},Normal)*RAD2DEG,Color {255,255,255,255});
                    }
                }

            }



        }

    }

    void update(int elaspedframes){

        if (cooldown<0 && !hasPlayed){
            hasPlayed=true;
                PlaySound(cut);
        }

        cooldown-=elaspedframes;
    }

private:

};


struct Splat {
    Vector2 pos;
    int radius;
    int angle;
    Splat(Vector2 p,int r){
        pos = p;
        radius = r;
        angle  = GetRandomValue(0,360);
    }
};

class WineSplater {
private:
public:
    int cooldown;
    bool hasPlayed = false;
    std::vector<Splat*> splats;

    WineSplater(){
        int Count = GetRandomValue(3,10);
        for (int i =0;i<Count;i++){
            Vector2 center = {(float)GetRandomValue(200,600),(float)GetRandomValue(100,350)};
            int radius =GetRandomValue(10,100);
            bool inRect = (center.x-radius>XLOWER) && (center.x+radius<XUPPER) && (center.y-radius>YLOWER) && (center.y+radius<YUPPER);
            if (inRect) {
                splats.push_back(new Splat(center,radius));
            }else {
                i--;
            }


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
            //DrawCircle(splats[i]->pos.x,splats[i]->pos.y,splats[i]->radius,Color {255,255,0,127});
            DrawTexturePro(wineLarge,Rectangle {0,0,128,128},Rectangle {splats[i]->pos.x,splats[i]->pos.y,(float)splats[i]->radius*2,(float)splats[i]->radius*2},Vector2 {(float)splats[i]->radius,(float)splats[i]->radius},splats[i]->angle,Color {255,255,255,255});



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
        if (!hasPlayed){
            hasPlayed=true;
            PlaySound(splat);

        }
        cooldown-=elaspedframes;
    }
};


bool menu(int& clock,int& high){
    bool out = false;
    if (IsKeyPressed(KEY_ENTER)){
        out =true;
    }

    if (IsGamepadAvailable(0)){
        if (IsGamepadButtonPressed(0,GAMEPAD_BUTTON_MIDDLE_RIGHT)){
            out =true;
        }
    }

    BeginDrawing();
    ClearBackground(SKYBLUE);


    if (high != 0){
        DrawText(("High Score: "+std::to_string(high/60)).c_str(), 200, 34, 24, ORANGE);
    }
    //DrawText(std::to_string(GetGamepadAxisMovement(0,0)).c_str(), 10, 56, 24, DARKGRAY);
    //DrawText(std::to_string(GetGamepadAxisMovement(0,1)).c_str(), 10, 70, 24, DARKGRAY);

    EndDrawing();

    return out;
}

enum Difficulty {
    EASY,
    NORMAL,
    MEDIUM,
    HARD,
    IMPOSSIBLE
};

int main_menu(Difficulty& diff){
    int out = -1;
    bool done = IsKeyPressed(KEY_ENTER);

    if (IsKeyPressed(KEY_A)){
        if (diff != Difficulty::EASY){
            diff = (Difficulty) ((int)diff - 1);
        }
    }


    if (IsKeyPressed(KEY_D)){
        if (diff !=Difficulty::IMPOSSIBLE){
            diff = (Difficulty) ((int)diff + 1);
        }
    }


    if (IsGamepadAvailable(0)){
        if (IsGamepadButtonPressed(0,GAMEPAD_BUTTON_LEFT_TRIGGER_1)){
            if (diff != Difficulty::EASY){
                diff = (Difficulty) ((int)diff - 1);
            }
        }
        if (IsGamepadButtonPressed(0,GAMEPAD_BUTTON_RIGHT_TRIGGER_1)){
            if (diff !=Difficulty::IMPOSSIBLE){
                diff = (Difficulty) ((int)diff + 1);
            }
        }

        if (!done){
            if (IsGamepadButtonPressed(0,GAMEPAD_BUTTON_MIDDLE_RIGHT)){
                done = true;
            }
        }

    }


    if (done){
        switch (diff){
            case Difficulty::EASY: {
                out = 8;
            }
                break;
            case Difficulty::NORMAL: {
                out = 15;
            }
                break;
            case Difficulty::MEDIUM: {
                out = 18;
            }
                break;
            case Difficulty::HARD: {
                out =20;
            }
                break;
            case Difficulty::IMPOSSIBLE: {
                out =65;
            }
                break;

        }

    }




    BeginDrawing();
    DrawTexturePro(mainMenu, (Rectangle){0, 0, 800, 450}, (Rectangle) {0, 0, (float) GetScreenWidth(), (float)GetScreenHeight()}, Vector2 {0, 0}, 0, WHITE);
    //DrawText("Main Menu",0,0,32,YELLOW);

    switch (diff){
        case Difficulty::EASY: {
            DrawTexturePro(difficulties, (Rectangle){0, 0, 120, 32}, (Rectangle){(float)(340*GetScreenWidth())/800, (float) (339*GetScreenHeight())/450, (float) (120*GetScreenWidth())/800, (float) (32*GetScreenHeight())/(450)}, (Vector2) {0, 0}, 0, WHITE);
            //DrawText("Easy",0,32,32,YELLOW);
        }
            break;
        case Difficulty::NORMAL: {
            DrawTexturePro(difficulties, (Rectangle){240, 0, 120, 32}, (Rectangle){(float)(340*GetScreenWidth())/800, (float) (339*GetScreenHeight())/450, (float) (120*GetScreenWidth())/800, (float) (32*GetScreenHeight())/(450)}, (Vector2) {0, 0}, 0, WHITE);
            //DrawText("Normal",0,32,32,YELLOW);
        }
            break;
        case Difficulty::MEDIUM: {
            DrawTexturePro(difficulties, (Rectangle){120, 0, 120, 32}, (Rectangle){(float)(340*GetScreenWidth())/800, (float) (339*GetScreenHeight())/450, (float) (120*GetScreenWidth())/800, (float) (32*GetScreenHeight())/(450)}, (Vector2) {0, 0}, 0, WHITE);
            //DrawText("Medium",0,32,32,YELLOW);
        }
            break;
        case Difficulty::HARD: {
            DrawTexturePro(difficulties, (Rectangle){360, 0, 120, 32}, (Rectangle){(float)(340*GetScreenWidth())/800, (float) (339*GetScreenHeight())/450, (float) (120*GetScreenWidth())/800, (float) (32*GetScreenHeight())/(450)}, (Vector2) {0, 0}, 0, WHITE);
            //DrawText("Hard",0,32,32,YELLOW);
        }
            break;
        case Difficulty::IMPOSSIBLE: {
            DrawTexturePro(difficulties, (Rectangle){480, 0, 120, 32}, (Rectangle){(float)(340*GetScreenWidth())/800, (float) (339*GetScreenHeight())/450, (float) (120*GetScreenWidth())/800, (float) (32*GetScreenHeight())/(450)}, (Vector2) {0, 0}, 0, WHITE);
            //DrawText("Impossible",0,32,32,YELLOW);
        }
            break;

    }

    EndDrawing();


    return out;
}


int main(){

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Say Cheese, Brie!");
    Image windowIcon = LoadImage("assets/cheese_icon.png");
    SetWindowIcon(windowIcon);

    InitAudioDevice();

    knife = LoadTexture("assets/knife.png");
    wineBig = LoadTexture("assets/wine_big.png");
    wineMedium = LoadTexture("assets/wine_medium.png");
    wineSmall = LoadTexture("assets/wine_small.png");
    waring = LoadTexture("assets/Slash_effect.png");
    dashTexture = LoadTexture("assets/dash_indicator.png");
    wineLarge = LoadTexture( "assets/wine_large.png");
    tutorial = LoadTexture("assets/menu.png");
    wine = LoadTexture("assets/wine_bottle.png");
    knifeFlat = LoadTexture( "assets/knifeStatic.png");
    tutorial2 = LoadTexture("assets/menu_controller.png");
    mainMenu = LoadTexture("assets/MENU_.png");
    difficulties = LoadTexture("assets/difficulty_names.png");


    Music Track1 = LoadMusicStream("assets/2023 game jam music v2.wav");
    Music Track2 = LoadMusicStream("assets/2023_game_jam_music_menu.wav");
    splat = LoadSound("assets/wine_splat.wav");
    dash = LoadSound("assets/2023_game_jam_dash_1.wav");
    cut= LoadSound("assets/knife_cut_sound_5.wav");
    cheese = LoadSound("assets/2023_game_jam_MMM_CHEESE.wav");
    SetSoundVolume(cut,0.5);
    SetSoundVolume(splat,1.5);
    SetMusicVolume(Track2,0.5);

    PlayMusicStream(Track2);



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

    bool hasAttack = false;

    bool menuflag = false;

    double time = GetTime();
    int elaspedframe = 1;

    RenderTexture2D renderTexture = LoadRenderTexture(SCREENWIDTH, SCREENHEIGHT);
    Texture2D background = LoadTexture("assets/Background_Plate_without_grain.png");

    int scale =-1;
    Difficulty difficulty = Difficulty::NORMAL;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(Track1);
        UpdateMusicStream(Track2);


        elaspedframe = 0;
        elaspedframe = (GetTime()-time)*60;
        time = GetTime();

        clock+=elaspedframe;

        //DrawText(std::to_string(GetGamepadButtonPressed()).c_str(), 10, 70, 24, DARKGRAY);

        attackTimerMax = -(scale*(std::log((float)clock/60.0f )/std::log(10)))+65;

        if (scale == -1){
            scale = main_menu(difficulty);
            if (scale != -1){
                for (int i = 0; i < attacks.size(); i++) {
                    delete attacks[i];
                }
                attacks.clear();
                menuflag = false;
                attackTimerMax = 65;
                clock = 0;
                player.totalElasped = 0;
                player.resetPlayer();
                wineCooldown = 0;
                if(wineSplater != nullptr){
                    delete wineSplater;
                    wineSplater = nullptr;
                }
                StopMusicStream(Track2);
                PlayMusicStream(Track1);
            }
            continue;
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
                wineCooldown = 0;
                if(wineSplater != nullptr){
                    delete wineSplater;
                    wineSplater = nullptr;
                }
                hasAttack = false;
                StopMusicStream(Track2); // stops menu music
                PlayMusicStream(Track1); // starts playing game music after game over
            } else {
                continue;

            }
        }



            if (wineSplater == nullptr && !menuflag) {
                wineCooldown+=elaspedframe;
            }

            if (wineCooldown >= 600) {
                wineSplater = new WineSplater();
                wineCooldown = 0;
            }

            if (clock > high) {
                high = clock;
            }






//            if (clock % scale == 0) {
//
//
//                attackTimerMax== ;
//            }


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
                hasAttack = true;
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
                PlaySound(cheese);
                StopMusicStream(Track1); // stops game music
                PlayMusicStream(Track2); // starts playing menu music on game over
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
            DrawTextureRec(tutorial, Rectangle {0,0,64,208},Vector2{XUPPER+8,YLOWER+8},WHITE);
            DrawTextureRec(wine, Rectangle {0,0,64,208},Vector2{XUPPER+8,YLOWER+208-8},WHITE);
            DrawTextureRec(tutorial2, Rectangle {0,0,64,208},Vector2{8,YLOWER+8},WHITE);
    //A change

        if (!hasAttack){
                DrawTextureRec(knifeFlat,Rectangle {0,0,32,192},Vector2{24-(float)attackTimer,YLOWER+208},WHITE);
            }

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
//
//            DrawText(("Attack timer max:" + std::to_string((attackTimerMax))).c_str(), 10, 94, 24, DARKGRAY);

            EndTextureMode();


            BeginDrawing();
                DrawTexturePro(renderTexture.texture,Rectangle{0,0,SCREENWIDTH,-SCREENHEIGHT},Rectangle{0,0,(float) GetScreenWidth(),(float) GetScreenHeight()},Vector2{0,0},0,WHITE);
            EndDrawing();



    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
