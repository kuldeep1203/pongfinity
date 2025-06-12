#include <raylib.h>
#include <iostream>
#include <string>

using namespace std;

const int screenwidth = 800;
const int screenheight = 800;



// class Match{
//     public: 
    
// }


class Button {
    public:
    Rectangle button;
    Button(float x, float y, float width, float height) {
        button = {x, y, width, height};
    }
    bool IsMouseOver() const {
        Vector2 mousePosition = GetMousePosition();
        return CheckCollisionPointRec(mousePosition, button);
    }
    void Draw(const char* label) const {
        bool hovered = IsMouseOver();
        DrawRectangleRec(button, hovered ? LIGHTGRAY : GRAY);
        DrawRectangleLinesEx(button, 2, DARKGRAY);  
        int fontSize = 20;
        int textWidth = MeasureText(label, fontSize);
        DrawText(label, button.x + (button.width - textWidth) / 2, button.y + 15, fontSize, BLACK);
    }
    void Draw() const {
        DrawRectangleRec(button, IsMouseOver() ? LIGHTGRAY : GRAY);
        DrawRectangleLinesEx(button, 2, DARKGRAY);  
    }
    bool IsButtonClicked() const {
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsMouseOver();
    }
 
  
   
};


class floatingWindow{
    public:

    bool floatingWindow = true;
    bool closewindow  = false;
    void Drawbasefloat(const char *str){
        DrawRectangle(200, 150, 400, 300, Fade(LIGHTGRAY, 0.9f));
        DrawRectangleLines(200, 150, 400, 300, DARKGRAY);
        DrawText(str, 280, 200, 20, DARKGRAY);
    }
  
    
   
};

class Ball {
public:
    int ball_x, ball_y;
    int ball_speed_x, ball_speed_y;
    int ball_radius;

    Ball(int s, int w) {
        ball_x = s / 2;
        ball_y = w / 2;
        ball_speed_x = 5;
        ball_speed_y = 5;
        ball_radius = 15;
    }

    
    int Update(Rectangle leftPaddle, Rectangle rightPaddle) {
        ball_x += ball_speed_x;
        ball_y += ball_speed_y;

        if (ball_y + ball_radius >= screenheight || ball_y - ball_radius <= 0) {
            ball_speed_y *= -1;
        }

       
        if (CheckCollisionCircleRec({(float)ball_x, (float)ball_y}, ball_radius, leftPaddle) ||
            CheckCollisionCircleRec({(float)ball_x, (float)ball_y}, ball_radius, rightPaddle)) {
            ball_speed_x *= -1;
        }

     
        if (ball_x - ball_radius > screenwidth) {
            return 1;
        }

      
        if (ball_x + ball_radius < 0) {
            return 2;
        }

        return 0;
    }

    void Reset() {
        ball_x = screenwidth / 2;
        ball_y = screenheight / 2;
        ball_speed_x = (ball_speed_x > 0) ? -5 : 5; 
        ball_speed_y = 5;
    }
};

class Paddle {
    int PaddlePoint;
public:
    Rectangle paddle;
   

    Paddle() {
        paddle = {0, screenheight / 2.0f - 50, 10, 100};
        PaddlePoint = 0;
    }
    int getPoint(){
        return PaddlePoint;
    }
    void Point(){
        PaddlePoint++;
    }

    void Movement() {
     
        if (IsKeyDown(KEY_W)) {
            paddle.y -= 5;
            if (paddle.y < 0) paddle.y = 0;
        }
        if (IsKeyDown(KEY_S)) {
            paddle.y += 5;
            if (paddle.y + paddle.height > screenheight)
                paddle.y = screenheight - paddle.height;
        }
    }
};

class CpuPaddle : public Paddle{
    int CpuPoint;
    public:
    
    CpuPaddle() {
        CpuPoint=0;
        paddle = {screenwidth - 10, screenheight / 2.0f - 50, 10, 100};
    }
    void Movement(int ballY) {
        float paddleCenterY = paddle.y + paddle.height / 2.0f;

        if (ballY < paddleCenterY - 10) {
            paddle.y -= 4;
            if (paddle.y < 0) paddle.y = 0;
        } else if (ballY > paddleCenterY + 10) {
            paddle.y += 4;
            if (paddle.y + paddle.height > screenheight)
                paddle.y = screenheight - paddle.height;
        }
    }
    int getPoint(){
        return CpuPoint;
    }
    void Point(){
        CpuPoint++;
    }


};


int main() {
    InitWindow(screenwidth, screenheight, "Pong Game");
    SetTargetFPS(60);

    Ball newBall(screenwidth, screenheight);
    Paddle paddle;
    CpuPaddle paddle2;
    floatingWindow floatingwindow ;
    Button Hostbutton(300, 250, 200, 60);
    Button Joinbutton(300, 350, 200, 60);
    bool HostbuttonClicked = false;
    bool JoinbuttonClicked = false;

    while (!WindowShouldClose()) {

        
        BeginDrawing();
        ClearBackground(BLACK);

        
        if(IsKeyPressed(KEY_SPACE)){
            floatingwindow.floatingWindow = !floatingwindow.floatingWindow;
           
        }
        if(floatingwindow.floatingWindow){
            floatingwindow.Drawbasefloat("Press 'Space' to start ");
            Hostbutton.Draw("Create Game");
            Joinbutton.Draw("Join Game");
          
          
           
           
        }
        else if (Hostbutton.IsButtonClicked()) {
            HostbuttonClicked = true;
            floatingwindow.floatingWindow = false;
        }
        else if (Joinbutton.IsButtonClicked()) {
            JoinbuttonClicked = true;
            floatingwindow.floatingWindow = false;
        }
        // else if(HostbuttonClicked){
        //     ClearBackground(BLACK);

        // }

        else{
            paddle.Movement();
            paddle2.Movement(newBall.ball_y);
            int scoreUpdate = newBall.Update(paddle.paddle, paddle2.paddle);
            if (scoreUpdate == 1) {
                paddle.Point();
                newBall.Reset();
            } else if (scoreUpdate == 2) {
                paddle2.Point();
                newBall.Reset();
            }
    
    
            DrawLine(screenwidth / 2, 0, screenwidth / 2, screenheight, WHITE);
    
            DrawRectangleRec(paddle.paddle, WHITE);
            DrawRectangleRec(paddle2.paddle, WHITE);
    
            DrawText(to_string(paddle.getPoint()).c_str(), screenwidth / 4, 2, 50, WHITE);
            DrawText(to_string(paddle2.getPoint()).c_str(), 3 * screenwidth / 4, 2, 50, WHITE);
    
            DrawCircle(newBall.ball_x, newBall.ball_y, newBall.ball_radius, WHITE);
            if(paddle.getPoint()==5 ){
                floatingwindow.Drawbasefloat("Congrats you won,Press 'Space' to start");
            }else if(paddle2.getPoint()==5 ){
                ClearBackground(BLACK);
                floatingwindow.Drawbasefloat("You lost:(,Press 'Space' to start");
            }
           
        }
       

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
