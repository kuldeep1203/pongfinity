#include<raylib.h>
#include<iostream>

using namespace std;

class Ball {
    public:
        int ball_x, ball_y;
        int ball_speed_x, ball_speed_y;
        int ball_radius;
    
        Ball(int s ,int w) {
            ball_x = s;
            ball_y = w;
            ball_speed_x = 5;
            ball_speed_y = 5;
            ball_radius = 15;
        }
};
    
int main(){

    const int screenwidth = 800;
    const int screenheight = 800;


    

    InitWindow(screenwidth,screenheight,"test window");

    SetTargetFPS(60);
    Ball newBall(screenwidth,screenheight);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        // newBall.ball_x+=newBall.ball_speed_x;
        // newBall.ball_y+=newBall.ball_speed_y;

        // if(newBall.ball_x + newBall.ball_radius >= screenwidth || newBall.ball_x-newBall.ball_radius <=0 ){
        //     newBall.ball_speed_x*=-1;
        // }
        // if(newBall.ball_y +newBall.ball_radius>=screenheight || newBall.ball_y-newBall.ball_radius<=0 ){
        //     newBall.ball_speed_y*=-1;
        // }

        DrawRectangle(790,screenheight/2,10,100,WHITE);
        DrawRectangle(0,screenheight/2,10,100,WHITE);
        DrawCircle(newBall.ball_x/2,newBall.ball_y/2,newBall.ball_radius,WHITE);

        EndDrawing();
    }
    CloseWindow();

    return 0;


}