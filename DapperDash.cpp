#include "raylib.h"


struct AnimData {

    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;

};

bool isOnGround(AnimData data, int windowHeight) {

    return data.pos.y >= windowHeight - data.rec.height;
};

AnimData updateAnimeData(AnimData data, float deltaTime, int maxFrame) {

    //update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime) {
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame) {

            data.frame = 0;
        }
    }
    return data;
};

int main() {

    //Window Dimensions
    int windowDimensions[2] = { 512, 380 };  

    //Initialize Window
    InitWindow(windowDimensions[0], windowDimensions[1], "SCARY ABDUL ATTACKS POOR SCARFMAN");

    //SCARFY!!!!!
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData{};
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0.0;
    scarfyData.rec.y = 0.0;
    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 12.0;
    scarfyData.runningTime = 0.0;

    //nebula :((((
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    //Array of nebulae
    const int sizeOfNebulae{ 12 };
    AnimData nebulae[sizeOfNebulae] {};



    for (int i = 0; i < sizeOfNebulae; i++) {
    
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].pos.x = windowDimensions[0] + i*300;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].updateTime = 1.0 / 16.0;
        nebulae[i].runningTime = 0.0;

        
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};
    
    int nebVel{ -200 }; //nebula x velocity (pixels/second)


    //Game Physics
    const int gravity{ 1'000 }; //Accelaration due to gravity (pixels/s^2)
    const int JumpVel{ -600 }; //Pixels per second
    int velocity{ 0 }; //Velocity of Scarfy
    bool isInAir{}; //Check to see if   n air 

    //Background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{}; 

    //midground
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};

    //foreground
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};


    SetTargetFPS(60); //you are retarded if you odn't know what this is

    bool collision{};

    //Game Logic begins here 
    while (!WindowShouldClose()) {
        const float dT{ GetFrameTime() }; // V and G consistency!!!
        //Start drawing
        BeginDrawing();
        //For good eyes
        ClearBackground(WHITE);

        bgX -= 40 * dT;
        mgX -= 60 * dT;
        fgX -= 80 * dT;

        if (bgX <= -background.width * 2) {

            bgX = 0.0;
        }
        if (mgX <= -midground.width * 2) {

            mgX = 0.0;
        }
        if (fgX <= -foreground.width * 2) {

            fgX = 0.0;
        }

        //DrawBackground
        Vector2 bg1Pos{ bgX, 0.0 };
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{ bgX + background.width * 2, 0.0 };
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        Vector2 mg1Pos{ mgX, 0.0 };
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{ mgX + midground.width * 2, 0.0 };
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        Vector2 fg1Pos{ fgX, 0.0 };
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{ fgX + foreground.width * 2, 0.0 };
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        //GROUNDED
        if (isOnGround(scarfyData, windowDimensions[1])) {
            //square is on ground
            velocity = 0;
            isInAir = false;
        }
        //In air
        else {
            //square is in the air 
            velocity += gravity * dT;
            isInAir = true;
        }

        //User input 
       if (IsKeyPressed(KEY_SPACE) && !isInAir) {
                //JUMP INTO IT
                velocity += JumpVel;
       }
       //update nebula position
       for (int i = 0; i < sizeOfNebulae; i++) {
           nebulae[i].pos.x += nebVel * dT;
       }
       //update position value if jump
       scarfyData.pos.y += velocity * dT;

       //update Finish Line
       finishLine += nebVel * dT;

        //Begin animation:
        //adding latency between frames to time of running
        scarfyData.runningTime += dT;
        //Check if Scarfy animation should be updated
        if (!isInAir) {
            scarfyData = updateAnimeData(scarfyData, dT, 5);
        }
            //Reset running time for next cycle
       

        for (int i = 0; i < sizeOfNebulae; i++) {

            nebulae[i] = updateAnimeData(nebulae[i], dT, 7);

        }

        
        for (AnimData nebula : nebulae) {
            float pad{ 40 };
            Rectangle nebRec{ 
                nebula.pos.x + pad, 
                nebula.pos.y + pad, 
                nebula.rec.width - 2 * pad, 
                nebula.rec.height - 2 * pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfyRec)) {

                collision = true;
            }
             
        }

        if (collision) {

           DrawText("GAME OVER!!!", windowDimensions[0]/4, windowDimensions[1]/2, 40.0, RED);
        }
        else if (scarfyData.pos.x >= finishLine) {
            DrawText("YOU WIN!!!", windowDimensions[0]/4, windowDimensions[1]/2, 40.0, RED);
        }
        else {
            for (int i = 0; i < sizeOfNebulae; i++) {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            //Draw SCARFY!!!!
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        //End the damn thing
        EndDrawing();
    }

    //Gotta do this for raylub, what can I guy do? T_T
    UnloadTexture(nebula);
    UnloadTexture(scarfy);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();

    //we all about that good c++ practices baby!
    return 0;
}