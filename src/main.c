#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROWS 300
#define COLS 400
#define FPS  60

/* some coordinate for tetromino */
typedef struct {
  int x;
  int y;
} Point;

static const Point tetromino[7][4][4] = {
  /* I block */
  {
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
    {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
    {{1, 0}, {1, 1}, {1, 2}, {1, 3}}
  },

  /* J block */
  {
    {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{1, 0}, {1, 1}, {1, 2}, {0, 2}},
    {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
    {{1, 0}, {2, 0}, {1, 1}, {1, 2}}
  },

  /* L block */
  {
    {{2, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
    {{0, 1}, {1, 1}, {2, 1}, {0, 2}},
    {{0, 0}, {1, 0}, {1, 1}, {1, 2}}
  },

  /* O block */
  {
    {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
    {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
    {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
    {{1, 0}, {2, 0}, {1, 1}, {2, 1}}
  },

  /* S block */
  {
    {{1, 0}, {2, 0}, {0, 1}, {1, 1}},
    {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
    {{1, 1}, {2, 1}, {0, 2}, {1, 2}},
    {{0, 0}, {0, 1}, {1, 1}, {1, 2}}
  },

  /* T block */
  {
    {{1, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{1, 1}, {1, 1}, {2, 1}, {1, 2}},
    {{0, 1}, {1, 1}, {2, 1}, {1, 2}},
    {{1, 0}, {0, 1}, {1, 1}, {1, 2}}
  },

  /* Z block */
  {
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
    {{2, 0}, {2, 1}, {1, 1}, {1, 2}},
    {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
    {{1, 0}, {0, 1}, {1, 1}, {0, 2}}
  }
};

/* Color constants */
static const Color colors[7] = {
  LIGHTGRAY,    // I
  BLUE,         // J
  ORANGE,       // L
  YELLOW,       // O
  GREEN,        // S
  PURPLE,       // T
  RED           // Z
};

/* Game state */
typedef struct {
  int grid[ROWS][COLS];     // 0 = empty, 1-7 = piece type
  int type;                 // 0-6
  int rot;                  // 0-3 rotate
  int x, y;                 // position of piece center
  
} Piece;

/* Globals */
Piece cur;
int dropCounter = 0;
int score = 0;
bool gameOver = false;

/* Game functions */
static void resetPiece(void) {
  cur.type = rand() % 7;
  cur.rot = 0;
  cur.x = COLS/2 - 2; // start in the middle
  cur.y = 0;          // top of screen
}

static int collision(int nx, int ny, int nrot) {
  for (int i = 0; i < 4; i++) {
    int gx = nx + tetronimo[cur.type][nrot][i].x;
    int gy = ny + tetronimo[cur.type][nrot][i].y;
    if (gx < 0 || gx >= COLS || gy < 0 || gy > ROWS) return 1;
    if (cur.grid[gx][gy]) return 1;
  }
  return 0;
}

static void lockPiece(void) {
  for (int i = 0; i < 4; i++) {
    int gx = cur.x + tetronimo[cur.type][cur.rot][i].x;
    int gy = cur.y + tetronimo[cur.type][cur.rot][i].y;
    cur.grid[gx][gy] = cur.type + 1;
  }

  /* clear full rows */
  for (int r = 0; r < ROWS; r++) {
    int full = 1;
    for (int c = 0; c < COLS; c++) {
      full &= cur.grid[r][c] != 0;
    }
    if (full) {
      /* shift everythingh down */
      for (int rr = r; rr > 0; rr--)
	memcpy(cur.grid[rr], cur.grid[rr-1], sizeof(int)*COLS);
      memset(cur.grid[0], 0, sizeof(int)*COLS);
      score += 100;
    }
  }
  resetPiece();
  if (collision(cur.x, cur.y, cur.rot))
    gameover = true;
}

static void hardDrop(void) {
  while(!collision(cur.x, cur.y+1, cur.rot))
    cur.y++;
  lockPiece();
}

static void rotatePiece(in dir) {
  int nrot = (cur.rot + dir) & 3;
  if (!collision(cur.x, cur.y, nrot)) cur.rot = nrot;
}

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
