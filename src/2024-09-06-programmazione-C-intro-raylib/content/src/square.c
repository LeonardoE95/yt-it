#include "raylib.h"
#include <stdio.h>

#define TARGET_FPS 60
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define SQUARE_SIZE 30

#define WINDOW_TITLE "raylib square example"

typedef struct Square {
  float length;
  float posX, posY;
  float speedX, speedY;
  Color col;
} Square;

void DrawSquare(Square *square) {
  DrawRectangle(square->posX, square->posY, square->length, square->length, square->col);
}

void GenerateSquare(Square *scene, int pos) {
  Color possibleColors[] = { BLACK, GREEN, DARKBLUE, DARKBROWN, MAGENTA };
  int colorSize = sizeof(possibleColors) / sizeof(Color);
  Color col = possibleColors[GetRandomValue(0, colorSize - 1)];

  float posX = GetRandomValue(0, WINDOW_WIDTH - SQUARE_SIZE);
  float posY = GetRandomValue(0, WINDOW_HEIGHT - SQUARE_SIZE);

  float speedX = GetRandomValue(0, 3);
  float speedY = GetRandomValue(0, 3);  

  scene[pos] = (Square) {
    .posX = posX, .posY = posY,
    .speedX = speedX, .speedY = speedY,
    .col = col,
    .length = SQUARE_SIZE,    
  };
}

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetRandomSeed(1337);
    SetTargetFPS(TARGET_FPS);

    int sceneSize = 100;
    int offset = 10;
    Square scene[sceneSize];

    for (int i = 0; i < offset; i++) {
      GenerateSquare(scene, i);
    }

    while (!WindowShouldClose()) {
      
      // Update
      //----------------------------------------------------------------------------------
      
      float dt = 100 * GetFrameTime();

      for (int i = 0; i < offset; i++) {
	Square *square = &scene[i];

	// handle movement
	square->posX += square->speedX * dt;
	square->posY += square->speedY * dt;	

	// handle collisions with borders
	if (square->posX < 0 || (square->posX + square->length) > WINDOW_WIDTH) {
	  square->speedX *= -1;
	}
	if (square->posY < 0 || (square->posY + square->length) > WINDOW_HEIGHT) {
	  square->speedY *= -1;
	}
	
      }

      // Input
      //----------------------------------------------------------------------------------
      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
	Vector2 mousePos = GetMousePosition();

	if (offset < sceneSize) {
	  GenerateSquare(scene, offset);
	  scene[offset].posX = mousePos.x; scene[offset].posY = mousePos.y;
	  scene[offset].speedX = 2; scene[offset].speedY = 0;
	  offset++;
	}

	if (offset < sceneSize) {
	  GenerateSquare(scene, offset);
	  scene[offset].posX = mousePos.x; scene[offset].posY = mousePos.y;
	  scene[offset].speedX = -2; scene[offset].speedY = 0;
	  offset++;
	}

	if (offset < sceneSize) {
	  GenerateSquare(scene, offset);
	  scene[offset].posX = mousePos.x; scene[offset].posY = mousePos.y;
	  scene[offset].speedX = 0; scene[offset].speedY = 2;
	  offset++;
	}

	if (offset < sceneSize) {
	  GenerateSquare(scene, offset);
	  scene[offset].posX = mousePos.x; scene[offset].posY = mousePos.y;
	  scene[offset].speedX = 0; scene[offset].speedY = -2;
	  offset++;
	}	
	
      }      

      // Drawing
      //----------------------------------------------------------------------------------      
      
      BeginDrawing();
        ClearBackground(RAYWHITE);
	for (int i = 0; i < offset; i++) {
	  DrawSquare(&scene[i]);	
	}
      EndDrawing();	
    }

    CloseWindow();

    return 0;
}
