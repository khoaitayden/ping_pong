
#include "raylib.h"
#include <stdio.h>

const int screenWidth = 800;
const int screenHeight = 450;
const int padWidth=10;
const int padHeight=100;
const int ballsize=10; 

int player1score = 0;
int player2score = 0;
bool botmode = false;
bool modechoosed = false;
float speedup=1.0f;
Sound hitwall;
Sound hitpad;
bool mute=false;
short songnum=1;
float volume=0.1f;
bool checkdarkmode=true;
bool choosedtheme=false;
float bothard=1;
bool choosedbotmode=false;
bool bosstrackplayed=false;

typedef struct Paddle {
    Rectangle rect;
    float speed;
} Paddle;

typedef struct Ball {
    Vector2 pos;
    Vector2 speed;
} Ball;



void padMove(Paddle *paddle, bool Up, bool Down){
    if (Up && paddle->rect.y > 0) paddle->rect.y -= paddle->speed*speedup;
    if (Down && paddle->rect.y < screenHeight - padHeight) paddle->rect.y += paddle->speed*speedup;
}

void ballMove (Ball *ball, Paddle *paddle1, Paddle *paddle2){
    ball->pos.x += ball->speed.x*speedup;
    ball->pos.y += ball->speed.y*speedup;
    //Ball hit wall
    if (ball->pos.y <= 0 || ball->pos.y >= screenHeight - ballsize)
    {
        PlaySound(hitwall);
        ball->speed.y *= -1;
        speedup +=0.05f;
    }

    // Ball hit pad
    if (CheckCollisionCircleRec(ball->pos, ballsize / 2, paddle1->rect) || CheckCollisionCircleRec(ball->pos, ballsize / 2, paddle2->rect))
    {
        PlaySound(hitpad);
        ball->speed.x *= -1;
        speedup +=0.05f;
        
    }
    if (ball->pos.x<=0)
    {
        player2score++;
        ball->pos = (Vector2){screenWidth / 2, screenHeight / 2};
        ball->speed = (Vector2){5.0f, 5.0f};
        speedup=1;
    }
    if (ball->pos.x>= screenWidth)
    {
        player1score++;
        ball->pos = (Vector2){screenWidth / 2, screenHeight / 2};
        ball->speed = (Vector2){-5.0f, -5.0f};
        speedup=1;
    }
        
};

void loadgame(Paddle *paddle1, Paddle *paddle2, Ball *ball){
    paddle1->rect = (Rectangle){20, screenHeight / 2 - padHeight / 2, padWidth, padHeight};
    paddle2->rect = (Rectangle){screenWidth - 20 - padWidth, screenHeight / 2 - padHeight / 2, padWidth, padHeight};
    paddle1->speed = 7.0f;
    paddle2->speed = 7.0f;

    ball->pos = (Vector2){screenWidth / 2, screenHeight / 2};
    ball->speed = (Vector2){5.0f, 5.0f};
};

void runbot(Paddle *botpad, Ball *ball){
    if (ball->pos.y > botpad->rect.y + padHeight / 2) 
    {
        botpad->rect.y += botpad->speed*bothard;
    } 
    else 
        if (ball->pos.y < botpad->rect.y + padHeight / 2) 
        {
        botpad->rect.y -= botpad->speed*bothard;
        }
};

void choosebotmode (){
    if (IsKeyPressed(KEY_FOUR)) 
    {
        bothard = 0.5f;
        choosedbotmode = true;
    } else
    if (IsKeyPressed(KEY_FIVE)) 
    {
        bothard = 1.0f;
        choosedbotmode = true;
    } else
    if (IsKeyPressed(KEY_SIX)) 
    { 
        bothard = 1.5f;
        choosedbotmode = true;
    } else
    if (IsKeyPressed(KEY_SEVEN)) 
    {
        bothard = 2.0f;
        choosedbotmode = true;
    } else
    if (IsKeyPressed(KEY_ZERO)) 
    {
        bothard = 10.0f;
        choosedbotmode = true;
    }
    
}



void choosegamemode (){
    if (IsKeyPressed(KEY_ONE)) 
    {
        botmode = false;
        modechoosed = true;
    }
    if (IsKeyPressed(KEY_TWO)) 
    {
        botmode = true;
        modechoosed = true;
    }
};

void scoreupdateblack(){
    char scoreText[20];
    sprintf(scoreText, "%d - %d", player1score, player2score);
    DrawText(scoreText, screenWidth / 2 - 50, 20, 40, RAYWHITE);
}

void scoreupdatewhite(){
    char scoreText[20];
    sprintf(scoreText, "%d - %d", player1score, player2score);
    DrawText(scoreText, screenWidth / 2 - 50, 20, 40, BLACK);
}

    Music soundtrack1;
    Music soundtrack2;
    Music soundtrack3;
    Music bosstrack;
void mastervolumn(){
    if (IsKeyPressed(KEY_M))
        {
            if (volume>=0.1f){volume=0;}else {volume+=0.02f;}
            SetMasterVolume(volume);
        }
}

void choosetheme(){
    if (!choosedtheme)
    {
        if (IsKeyPressed(KEY_F))
        {
            checkdarkmode=false;
            choosedtheme=true;
        } 
        if (IsKeyPressed(KEY_B)){
            checkdarkmode=true;
            choosedtheme=true;
        }
    }
}

void loadsong(){
    if (IsKeyPressed(KEY_N))
        {
            StopMusicStream(soundtrack1);
            StopMusicStream(soundtrack2);
            StopMusicStream(soundtrack3);
            StopMusicStream(bosstrack);
            if (songnum==3){songnum=1;}else {songnum++;}
            switch (songnum){
                case 1:PlayMusicStream(soundtrack1);break;
                case 2:PlayMusicStream(soundtrack2);break;
                case 3:PlayMusicStream(soundtrack3);break;
            }
            bosstrackplayed=false;
        }
}

void bossmusic(){
        StopMusicStream(soundtrack1);
        StopMusicStream(soundtrack2);
        StopMusicStream(soundtrack3);
        PlayMusicStream(bosstrack);
}

int main(void)
{
    Paddle paddle1, paddle2;
    Ball ball;
    loadgame(&paddle1, &paddle2, &ball);
    InitWindow(screenWidth, screenHeight, "Ping Pong");
    InitAudioDevice();
    
    soundtrack1=LoadMusicStream("ping_pong_sound_track.mp3");
    soundtrack2=LoadMusicStream("ballsong.mp3");
    soundtrack3=LoadMusicStream("goodsong.mp3");
    bosstrack=LoadMusicStream("bosstrack.mp3");
    hitwall=LoadSound("hit wall.wav");
    hitpad=LoadSound("hit pad.wav");
    SetTargetFPS(90);
    
    while (!WindowShouldClose())
    {
        choosetheme();
        if (choosedtheme)
        {
            mastervolumn();
            loadsong();
            UpdateMusicStream(soundtrack1);
            UpdateMusicStream(soundtrack2);
            UpdateMusicStream(soundtrack3);
            if (checkdarkmode)
            {    
                if (!modechoosed)choosegamemode();
                if (modechoosed)
                 {
                    scoreupdateblack();
                    if (botmode)
                    {
                        choosebotmode();
                        if (choosedbotmode)
                        {
                            if (bothard>5 && !bosstrackplayed){
                                bossmusic();
                                bosstrackplayed=true;
                            }
                            UpdateMusicStream(bosstrack);
                            BeginDrawing();
                                ClearBackground(BLACK);
                                DrawRectangleRec(paddle1.rect, RED);
                                DrawRectangleRec(paddle2.rect, RED);
                                DrawCircleV(ball.pos, ballsize / 2, WHITE);
                            EndDrawing();
                            padMove(&paddle1,IsKeyDown(KEY_W), IsKeyDown(KEY_S));
                            runbot (&paddle2,&ball);
                        } 
                        else 
                        {
                            BeginDrawing();
                                ClearBackground(BLACK);
                                DrawText("Ping Pong", screenWidth / 2 - 120, screenHeight / 4 - 60, 50, RAYWHITE);
                                DrawText("Press '4' easy Mode", screenWidth / 2 - 140, screenHeight / 2 + 60, 20, RAYWHITE);
                                DrawText("Press '5' medium Mode", screenWidth / 2 - 140, screenHeight / 2 + 90, 20, RAYWHITE);
                                DrawText("Press '6' hard Mode", screenWidth / 2 - 140, screenHeight / 2 + 120, 20, RAYWHITE);
                                DrawText("Press '7' nightmate Mode", screenWidth / 2 - 140, screenHeight / 2 + 150, 20, RAYWHITE);
                                DrawText("Press '0' why even bother", screenWidth / 2 - 140, screenHeight / 2 + 180, 20, RAYWHITE);
                            EndDrawing();
                        };
                    } 
                    else 
                    {
                        BeginDrawing();
                            ClearBackground(BLACK);
                            DrawRectangleRec(paddle1.rect, RED);
                            DrawRectangleRec(paddle2.rect, RED);
                            DrawCircleV(ball.pos, ballsize / 2, WHITE);
                        EndDrawing();
                        padMove(&paddle1,IsKeyDown(KEY_W), IsKeyDown(KEY_S));
                        padMove(&paddle2,IsKeyDown(KEY_UP), IsKeyDown(KEY_DOWN));
                        bosstrackplayed=false;
                        StopMusicStream(bosstrack);
                    };
                    
                    if ((botmode&&choosedbotmode)||!botmode){
                        if (player1score >= 10 || player2score >= 10)
                        {
                            if (player1score >= 10) 
                            {
                                DrawText("Player 1 Win", screenWidth / 2 - 100, screenHeight / 2 - 30, 50, RAYWHITE);
                            } else 
                            {
                                DrawText("Player 2 Win", screenWidth / 2 - 100, screenHeight / 2 - 30, 50, RAYWHITE);
                            }
                            DrawText("Press R to Restart", screenWidth / 2 - 120, screenHeight / 2 + 30, 20, RAYWHITE);
                            if (IsKeyPressed(KEY_R)) {
                                player1score = 0;
                                player2score = 0;
                                speedup=1;
                            }
                        } else 
                        {
                            ballMove(&ball, &paddle1, &paddle2);
                        }
                    }
                    //choose mode again
                    if (IsKeyDown(KEY_L))
                    {
                           modechoosed = false;
                           player1score = 0;
                           player2score = 0;
                           speedup=1;
                           choosedbotmode=false;
                           bosstrackplayed=false;
                           StopMusicStream(bosstrack);
                    }
                } 
                else 
                {
                BeginDrawing();
                    ClearBackground(BLACK);
                    DrawText("Ping Pong", screenWidth / 2 - 120, screenHeight / 4 - 60, 50, RAYWHITE);
                    DrawText("Press '1' for Friend Mode", screenWidth / 2 - 140, screenHeight / 2 + 90, 20, RAYWHITE);
                    DrawText("Press '2' for Bot Mode", screenWidth / 2 - 140, screenHeight / 2 + 110, 20, RAYWHITE);
                    DrawText("Press 'L' to choose mode again", 20, 430, 10, RAYWHITE);
                    DrawText("Press 'M' to change volume", 650, 430, 10, RAYWHITE);
                    DrawText("Press 'N' to choose song", 350, 430, 10, RAYWHITE);
                EndDrawing();
                }
            } 
            else
            {
                if (!modechoosed)choosegamemode();
                if (modechoosed)
                {
                   
                    scoreupdatewhite();
                    if (botmode)
                    {
                        choosebotmode();                 
                        if (choosedbotmode)
                        {
                            if (bothard>5 && !bosstrackplayed){
                                bossmusic();
                                bosstrackplayed=true;
                            }   
                            UpdateMusicStream(bosstrack);
                        BeginDrawing();
                            ClearBackground(WHITE);
                            DrawRectangleRec(paddle1.rect, RED);
                            DrawRectangleRec(paddle2.rect, RED);
                            DrawCircleV(ball.pos, ballsize / 2, BLACK);
                        EndDrawing();
                        padMove(&paddle1,IsKeyDown(KEY_W), IsKeyDown(KEY_S));
                        runbot (&paddle2,&ball);
                        }
                        else
                        {
                            BeginDrawing();
                                ClearBackground(WHITE);
                                DrawText("Ping Pong", screenWidth / 2 - 120, screenHeight / 4 - 60, 50, BLACK);
                                DrawText("Press '4' easy Mode", screenWidth / 2 - 140, screenHeight / 2 + 60, 20, BLACK);
                                DrawText("Press '5' medium Mode", screenWidth / 2 - 140, screenHeight / 2 + 90, 20, BLACK);
                                DrawText("Press '6' hard Mode", screenWidth / 2 - 140, screenHeight / 2 + 120, 20, BLACK);
                                DrawText("Press '7' nightmate Mode", screenWidth / 2 - 140, screenHeight / 2 + 150, 20, BLACK);
                                DrawText("Press '0' why even bother", screenWidth / 2 - 140, screenHeight / 2 + 180, 20, BLACK);
                            EndDrawing();
                            bosstrackplayed=false;
                            StopMusicStream(bosstrack);                            
                        }
                    } 
                    else 
                    {
                        bosstrackplayed=false;
                        StopMusicStream(bosstrack);
                        BeginDrawing();
                            ClearBackground(WHITE);
                            DrawRectangleRec(paddle1.rect, RED);
                            DrawRectangleRec(paddle2.rect, RED);
                            DrawCircleV(ball.pos, ballsize / 2, BLACK);
                        EndDrawing();
                        padMove(&paddle1,IsKeyDown(KEY_W), IsKeyDown(KEY_S));
                        padMove(&paddle2,IsKeyDown(KEY_UP), IsKeyDown(KEY_DOWN));
                        bosstrackplayed=false;
                        StopMusicStream(bosstrack); 
                    };
                    
                    if ((botmode&&choosedbotmode)||!botmode){
                        if (player1score >= 10 || player2score >= 10)
                        {
                            if (player1score >= 10) 
                            {
                                DrawText("Player 1 Win", screenWidth / 2 - 100, screenHeight / 2 - 30, 50, BLACK);
                            } else 
                            {
                                DrawText("Player 2 Win", screenWidth / 2 - 100, screenHeight / 2 - 30, 50, BLACK);
                            }
                            DrawText("Press R to Restart", screenWidth / 2 - 120, screenHeight / 2 + 30, 20, BLACK);
                            if (IsKeyPressed(KEY_R)) {
                                player1score = 0;
                                player2score = 0;
                                speedup=1;
                            }
                        } else 
                        {
                            ballMove(&ball, &paddle1, &paddle2);
                        }
                    }
                    //choose mode again
                    if (IsKeyDown(KEY_L))
                    {
                           modechoosed = false;
                           player1score = 0;
                           player2score = 0;
                           speedup=1;
                           bosstrackplayed=false;
                           StopMusicStream(bosstrack);
                           choosedbotmode=false;
                    }
                } 
                else 
                {
                BeginDrawing();
                    ClearBackground(WHITE);
                    DrawText("Ping Pong", screenWidth / 2 - 120, screenHeight / 4 - 60, 50, BLACK);
                    DrawText("Press '1' for Friend Mode", screenWidth / 2 - 140, screenHeight / 2 + 90, 20, BLACK);
                    DrawText("Press '2' for Bot Mode", screenWidth / 2 - 140, screenHeight / 2 + 110, 20, BLACK);
                    DrawText("Press 'L' to choose mode again", 20, 430, 10, BLACK);
                    DrawText("Press 'M' to change volume", 650, 430, 10, BLACK);
                    DrawText("Press 'N' to choose song", 350, 430, 10, BLACK);
                EndDrawing();
                }
            }
        } 
        else
        {
            BeginDrawing();
                    ClearBackground(BLACK);
                    DrawText("Ping Pong", screenWidth / 2 - 120, screenHeight / 4 - 60, 50, WHITE);
                    DrawText("Press 'F' for light mode", screenWidth / 2 - 140, screenHeight / 2 + 90, 20, WHITE);
                    DrawText("Press 'B' for dark mode", screenWidth / 2 - 140, screenHeight / 2 + 110, 20, WHITE);
                EndDrawing();
        }
    }
    UnloadMusicStream(soundtrack1);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}