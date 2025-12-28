#include "raylib.h"
#include <stdlib.h>

#define ROWS 300
#define COLS 400
#define FPS  60

int main(void) {
  InitWindow(COLS, ROWS, "Tetris - Raylib");
  SetTargetFPS(FPS);
  
  while (!WindowShouldClose()) {
    BeginDrawing(); // start drawing
    ClearBackground(RAYWHITE); // clear background
    EndDrawing();
  }

  CloseWindow();
}
