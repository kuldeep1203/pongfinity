#include <raylib.h>
#include<raymath.h>
#include <cmath>
#include <iostream>

using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const float centerX = screenWidth / 2.0f;
const float centerY = screenHeight / 2.0f;
const float arenaRadius = 300.0f;

struct Ball {
    Vector2 position;
    Vector2 velocity;
    float radius = 8.0f;

    Ball() {
        position = {centerX, centerY};
        velocity = {3.5f, 2.5f};
    }

    void Update() {
        position.x += velocity.x;
        position.y += velocity.y;

        float dx = position.x - centerX;
        float dy = position.y - centerY;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist + radius >= arenaRadius) {
            Vector2 normal = Vector2Normalize({dx, dy});//normailizaing  will give us the direction 
            float dot = velocity.x * normal.x + velocity.y * normal.y;
            velocity.x -= 2 * dot * normal.x;
            velocity.y -= 2 * dot * normal.y;
        }
    }

    void Draw() {
        DrawCircleV(position, radius, RAYWHITE);
    }
};

class Paddle {
public:
    float angle;           
    float angularSpeed; 
    float paddleLength;    

    Paddle(float startAngle) {
        angle = startAngle;
        angularSpeed = 0.025f;
        paddleLength = 50.0f; 
    }

    void Move(bool clockwise) {
        if (clockwise)
            angle += angularSpeed;
        else
            angle -= angularSpeed;

        //normalizing the angle between 0 and 2pi
        if (angle >= 2 * PI) angle -= 2 * PI;
        if (angle < 0) angle += 2 * PI;
    }

    void Draw() {
        Vector2 p1 = {
            centerX + (arenaRadius - 10) * cosf(angle),
            centerY + (arenaRadius - 10) * sinf(angle)
        };
        Vector2 p2 = {
            //arcleng =  radius * angle(radians)
            centerX + (arenaRadius - 10) * cosf(angle + paddleLength / arenaRadius),
            centerY + (arenaRadius - 10) * sinf(angle + paddleLength / arenaRadius)
        };
        DrawLineEx(p1, p2, 8.0f, GREEN);
    }
    

    // It is at an angle covered by the paddle on the circle 
    // (within the paddle's arc).
    // Its outer edge is touching or past the paddle's radius on the circle.

    bool CheckCollision(Vector2 ballPos, float ballRadius) {
        float dx = ballPos.x - centerX;
        float dy = ballPos.y - centerY;
        float ballAngle = atan2f(dy, dx);
        //atan2f(dy, dx) returns the angle (in radians) 
        //of the ball's position relative to the positive x-axis.
        if (ballAngle < 0) ballAngle += 2 * PI;

        float delta = fabsf(ballAngle - angle);
        if (delta > PI) delta = 2 * PI - delta;

        float dist = sqrtf(dx * dx + dy * dy);
        return delta < (paddleLength / arenaRadius) && dist + ballRadius >= arenaRadius - 12;
    }
};

void AIMove(Paddle &ai, Vector2 ballPos) {
    float dx = ballPos.x - centerX;
    float dy = ballPos.y - centerY;
    float ballAngle = atan2f(dy, dx);
    if (ballAngle < 0) ballAngle += 2 * PI;

    float diff = ballAngle - ai.angle;
    if (diff > PI) diff -= 2 * PI;
    if (diff < -PI) diff += 2 * PI;

    if (diff > 0.05f)
        ai.Move(true);
    else if (diff < -0.05f)
        ai.Move(false);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Circular Pong");
    SetTargetFPS(60);

    Ball ball;
    Paddle player(PI / 2),ai(PI/2);       // Top`

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_LEFT)) player.Move(false);
        if (IsKeyDown(KEY_RIGHT)) player.Move(true);

        AIMove(ai, ball.position);
        ball.Update();

        if (player.CheckCollision(ball.position, ball.radius) ||
            ai.CheckCollision(ball.position, ball.radius)) {
            ball.velocity = Vector2Scale(ball.velocity, -1.0f);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircleLines(centerX, centerY, arenaRadius, DARKGRAY);
        player.Draw();
        ai.Draw();
        ball.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


