#include <iostream>
#include "raylib.h"

int player_score{};
int enemy_score{};

class Ball {
public:
	float x, y;
	int speed_x, speed_y;
	int radius;

	void DrawBall() {
		DrawCircle(x, y, radius, WHITE);
	}

	void Update() {

		x += speed_x;
		y += speed_y;

		if (y + radius >= GetScreenHeight() || y - radius <= 0)
		{
			speed_y *= -1;
		}

		if (x + radius >= GetScreenWidth())
		{
			enemy_score++;
			ResetBall();
		}

		if (x - radius <= 0)
		{
			player_score++;
			ResetBall();
		}
	}

	void ResetBall() {

		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speed_choices[2] = {-1 , 1};
		speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];

	}
};

class Player {
protected:

	void LimitMovement() {
		if (y <= 0)
		{
			y = 0;
		}
		if (y + height >= GetScreenHeight())
		{
			y = GetScreenHeight() - height;
		}
	}

public:
	float x, y;
	int speed;
	float width,height;

	void Draw() {
		DrawRectangleRounded(Rectangle{x,y, width, height}, 0.8, 0, WHITE);
	}

	void Update() {
		// Event Handing
		if (IsKeyDown(KEY_W))
		{
			y = y - speed;
		}
		if (IsKeyDown(KEY_S))
		{
			y = y + speed;
		}

		LimitMovement();
	}
};


class CPUEnemy : public Player {
public:

	void Update(int ball_y)
	{
		if (y + height / 2 > ball_y)
		{
			y = y - speed;
		}
		if (y - height / 2 <= ball_y)
		{
			y = y + speed;
		}
		LimitMovement();
	}
};

Ball ball;
Player player;
CPUEnemy enemy;

int main() {
	const int WINDOW_HEIGHT = 720;
	const int WINDOW_WIDTH = 1280;
	int GameFPS = 60;

	int ball_Y = WINDOW_HEIGHT / 2;
	int ball_X = WINDOW_WIDTH / 2;

	player.width = 25;
	player.height = 120;
	player.x = WINDOW_WIDTH - player.width -10;
	player.y = WINDOW_HEIGHT/2 - player.height/2;
	player.speed = 10;


	enemy.width = 25;
	enemy.height = 120;
	enemy.x = 10;
	enemy.y = WINDOW_HEIGHT / 2 - 60;
	enemy.speed = 10;

	ball.radius = 20;
	ball.x = 25;
	ball.y = WINDOW_HEIGHT / 2 - enemy.height / 2;
	ball.speed_x = 7;
	ball.speed_y = 7;

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong"); // Creating The window
	SetTargetFPS(GameFPS);

	//GameLoop
	while (WindowShouldClose() == false)
	{
		// Updating Pos
		ball.Update();
		player.Update();
		enemy.Update(ball.y);

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height }))
		{
			ball.speed_x *= -1;
			ball.x = player.x - ball.radius - 1;  // push left
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ enemy.x, enemy.y, enemy.width, enemy.height }))
		{
			ball.speed_x *= -1;
			ball.x = enemy.x + enemy.width + ball.radius + 1;  // push right
		}




		//Drawing
		BeginDrawing(); // Begin drawing to canvas aka screen
		ClearBackground(BLACK); //Clearing the background everyframe so the ball doesn't leave a long trace

		DrawLine(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT, WHITE);
		player.Draw();
		enemy.Draw();
		ball.DrawBall();
		DrawText(TextFormat("%i" ,enemy_score), WINDOW_WIDTH / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", player_score),3 * WINDOW_WIDTH / 4 - 20, 20, 80, WHITE);
		EndDrawing(); // Ending drawing every frame
	}

	CloseWindow(); // Closing the window
	return 0;
}