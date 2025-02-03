
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

typedef struct Paddle {
    Rectangle rect;
    float speed;
} Paddle;

typedef struct Ball {
    Vector2 pos;
    Vector2 speed;
} Ball;



void padMove(Paddle *paddle, bool Up, bool Down){
    if (Up && paddle->rect.y > 0) paddle->rect.y -= paddle->speed;
    if (Down && paddle->rect.y < screenHeight - padHeight) paddle->rect.y += paddle->speed;
}

void ballMove (Ball *ball, Paddle *paddle1, Paddle *paddle2){
    ball->pos.x += ball->speed.x*speedup;
    ball->pos.y += ball->speed.y*speedup;
    //Ball hit wall
    if (ball->pos.y <= 0 || ball->pos.y >= screenHeight - ballsize)
    {
        ball->speed.y *= -1;
    }

    // Ball hit pad
    if (CheckCollisionCircleRec(ball->pos, ballsize / 2, paddle1->rect) || CheckCollisionCircleRec(ball->pos, ballsize / 2, paddle2->rect))
    {
        ball->speed.x *= -1;
    }
    if (ball->pos.x<=0)
    {
        player1score++;
        ball->pos = (Vector2){screenWidth / 2, screenHeight / 2};
        ball->speed = (Vector2){5.0f, 5.0f};
    }
    if (ball->pos.x>= screenWidth)
    {
        player2score++;
        ball->pos = (Vector2){screenWidth / 2, screenHeight / 2};
        ball->speed = (Vector2){-5.0f, -5.0f};
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
        botpad->rect.y += botpad->speed;
    } 
    else 
        if (ball->pos.y < botpad->rect.y + padHeight / 2) 
        {
        botpad->rect.y -= botpad->speed;
        }
};

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

void scoreupdate(){
    char scoreText[20];
    sprintf(scoreText, "%d - %d", player1score, player2score);
    DrawText(scoreText, screenWidth / 2 - 50, 20, 40, RAYWHITE);
}

int main(void)
{
    
    Paddle paddle1, paddle2;
    Ball ball;
    loadgame(&paddle1, &paddle2, &ball);
    InitWindow(screenWidth, screenHeight, "Ping Pong");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        choosegamemode();
        
        if (modechoosed){
            BeginDrawing();
                ClearBackground(BLACK);
                DrawRectangleRec(paddle1.rect, RED);
                DrawRectangleRec(paddle2.rect, RED);
                DrawCircleV(ball.pos, ballsize / 2, WHITE);
            EndDrawing();
            scoreupdate();
            if (botmode)
            {
                padMove(&paddle1,IsKeyDown(KEY_W), IsKeyDown(KEY_S));
                runbot (&paddle2,&ball);
            } 
            else 
            {
                padMove(&paddle1,IsKeyDown(KEY_W), IsKeyDown(KEY_S));
                padMove(&paddle2,IsKeyDown(KEY_UP), IsKeyDown(KEY_DOWN));
            };
            if (player1score >= 10 || player2score >= 10)
            {
                if (player1score >= 10) 
                {
                    DrawText("Player 2 Win", screenWidth / 2 - 100, screenHeight / 2 - 30, 50, RAYWHITE);
                } else 
                {
                    DrawText("Player 1 Win", screenWidth / 2 - 100, screenHeight / 2 - 30, 50, RAYWHITE);
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
                speedup +=GetFrameTime()*0.01f;
            }
            //choose mode again
            if (IsKeyDown(KEY_L))
            {
                   modechoosed = false;
                   player1score = 0;
                   player2score = 0;
                   speedup=1;
            }
        } 
        else 
        {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Ping Pong", screenWidth / 2 - 120, screenHeight / 4 - 60, 50, RAYWHITE);
            DrawText("Press '1' for Friend Mode", screenWidth / 2 - 140, screenHeight / 2 + 100, 20, RAYWHITE);
            DrawText("Press '2' for Bot Mode", screenWidth / 2 - 140, screenHeight / 2 + 130, 20, RAYWHITE);
            DrawText("Press 'L' to choose mode again", 20, 430, 10, RAYWHITE);
        EndDrawing();
        }
    //CloseWindow();    
    }
    return 0;
}