#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <string.h>

// ---------------------------------------------------------------------------

#define GRID_FILENAME "./data/board1.txt"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define BOARD_ROWS 9
#define BOARD_COLS 9

#define CELL_WIDTH (SCREEN_WIDTH / BOARD_COLS)
#define CELL_HEIGHT (SCREEN_HEIGHT / BOARD_ROWS)

// ---------------------------------------------------------------------------

typedef enum {
  V_NONE = 0,
  V_1,
  V_2,
  V_3,
  V_4,
  V_5,
  V_6,
  V_7,
  V_8,
  V_9,
  V_INVALID
} CellValue;

typedef enum {
  FIXED = 0,
  DYNAMIC
} CellType;

typedef struct {
  CellValue value;
  CellType type;
} Cell;

typedef enum {
  D_NONE = 0,
  D_UP,  
  D_DOWN,
  D_LEFT,
  D_RIGHT  
} Dir;

typedef struct {
  size_t x;
  size_t y;
} Pos;

typedef enum {
  E_NONE = 0,
  E_ROW,
  E_COL,
  E_SQUARE,
} ErrorType;

typedef struct {
  ErrorType type;
  size_t index;
} Error;

typedef struct {
  size_t cols;
  size_t rows;
  Cell *grid;

  Error error;
  Pos select;
  uint8_t victory;
} Game;

#define HAS_SELECTED_CELL(g) (((g).select.x != -1) && ((g).select.y != -1))
#define HAS_VALUE(c) (((c).value != V_INVALID) && ((c).value != V_NONE))
#define CELL_IS_DYNAMIC(g, pos)                                                \
  ((g).grid[(pos).y * (g).cols + (pos).x].type == DYNAMIC)
#define SELECTED_CELL_IS_DYNAMIC(g) CELL_IS_DYNAMIC((g), (g).select)
#define IS_OVER(G) ((g).victory == 1)

// ---------------------------------------------------------------------------

Game game_init(size_t rows, size_t cols);
Game game_init_from_file(const char *filename);
void game_close(Game g);
void game_fill(Game g, CellValue val);

Game game_check_and_set_selection(Game g, Pos pos);
Game game_check_and_set_value(Game g, Cell c);
Game game_check_and_set_error(Game g);
Game game_check_and_set_victory(Game g);

CellValue char_to_value(char keyPress);
Pos coords_to_pos(Vector2 pos);
Pos next_pos(Game g, Dir d);

void game_render_lines(Game g);
void game_render_values(Game G);
void game_render_error(Game G);
void game_render(Game g);

// ---------------------------------------------------------------------------

Game game_init(size_t rows, size_t cols) {
  Game g = { 0 };
  g.rows = rows;  
  g.cols = cols;
  g.grid = calloc(cols * rows, sizeof(Cell));
  g.select = (Pos){-1, -1};
  return g;
}

Game game_init_from_file(const char *filename) {
  FILE *f = fopen(filename, "r");

  if (f == NULL) {
    fprintf(stderr, "ERROR: could not open file %s: `%s`\n", filename, strerror(errno));
    exit(1);
  }

  char *line;
  size_t len = 0;
  size_t read;

  read = getline(&line, &len, f);

  if (read == -1) {
    fprintf(stderr, "ERROR: could not read line %d from %s: %s\n", 0, filename, strerror(errno));
    exit(1);    
  }

  char *rows_str = strtok(line, ",");
  char *cols_str = strtok(NULL, ",");
  char *null_str = strtok(NULL, ",");

  if (!rows_str || !cols_str || null_str) {
    fprintf(stderr, "ERROR: grid file not properly formatted %s\n", filename);
    exit(1);    
  }

  int rows, cols;
  rows = atoi(rows_str);
  cols = atoi(cols_str);

  if (!rows) {
    fprintf(stderr, "ERROR: could not atoi rows value (%s)\n", rows_str);
    exit(1);        
  }

  if (!cols) {
    fprintf(stderr, "ERROR: could not atoi cols value (%s)\n", rows_str);    
    exit(1);        
  }

  Game g = game_init(rows, cols);

  for (size_t y = 0; y < g.rows; y++) {
    read = getline(&line, &len, f);

    if (read == -1) {
      fprintf(stderr, "ERROR: could not read line %zu from %s: %s\n", y, filename, strerror(errno));
      exit(1);
    }

    for (size_t x = 0; x < g.cols; x++) {
      char *value_str = strtok(x == 0 ? line : NULL, ",");
      CellValue val;

      if (*value_str == 'X') {
	// empty cell
	g.grid[y * g.cols + x] = (Cell) { V_NONE, DYNAMIC };
      } else if((val = char_to_value(*value_str)) != V_INVALID) {
	// put val
	g.grid[y * g.cols + x] = (Cell) { val, FIXED };
      } else {
	// invalid
	fprintf(stderr, "ERROR: Invalid value found (%c) when reading file %s\n", *value_str, filename);
	exit(1);	
      }      
    }    
  }

  fclose(f);

  return g;
}

void game_fill(Game g, CellValue val) {
  for (size_t y = 0; y < g.rows; y++) {
    for (size_t x = 0; x < g.cols; x++) {
      g.grid[y * g.cols + x] = (Cell) { val, DYNAMIC };
    }
  }
}

Game game_check_and_set_selection(Game g, Pos pos) {
  if (CELL_IS_DYNAMIC(g, pos)) {
    g.select = pos;
  }  
  return g;    
}

Game game_check_and_set_value(Game g, Cell c) {
  if (HAS_SELECTED_CELL(g) && SELECTED_CELL_IS_DYNAMIC(g) && (c.value != V_INVALID)) {
    g.grid[g.select.y * g.cols + g.select.x] = c;
  }  
  return g;
}

Game game_check_and_set_error(Game g) {
  g.error = (Error) { E_NONE, -1 };

  // each row, column and square must contain 9 distinct digits, from 1 through 9

  // check row
  for (int y = 0; y < g.rows; y++) {
    size_t seen[10] = { 0 };
    for (int x = 0; x < g.cols; x++) {
      Cell c = g.grid[y * g.cols + x];
      if (HAS_VALUE(c)) {
	seen[c.value] += 1;
	if (seen[c.value] > 1) {
	  g.error = (Error) { E_ROW, y };
	  return g;
	}
      }
    }
  }

  // check col
  for (int x = 0; x < g.cols; x++) {
    size_t seen[10] = { 0 };
    for (int y = 0; y < g.rows; y++) {
      Cell c = g.grid[y * g.cols + x];
      if (HAS_VALUE(c)) {
	seen[c.value] += 1;
	if (seen[c.value] > 1) {
	  g.error = (Error) { E_COL, x };
	  return g;
	}
      }
    }
  }  

  // check square
  
  // TODO: for now we're hard-coding this to size 9x9, in the future
  // we can make it more general.
  if (BOARD_COLS == BOARD_ROWS) {
    for (int i = 0; i < 9; i++) {

      size_t seen[10] = { 0 };

      size_t start_y = floorf(i / 3) * 3;
      size_t start_x = (i % 3) * 3;

      for (size_t y = start_y; y < start_y + 3; y++) {
	for (size_t x = start_x; x < start_x + 3; x++) {
	  Cell c = g.grid[y * g.cols + x];
	  if (HAS_VALUE(c)) {
	    seen[c.value] += 1;
	    if (seen[c.value] > 1) {
	      g.error = (Error) { E_SQUARE, i };
	      return g;
	    }
	  }	  
	}
      }
      
    }
  }

  return g;
}

Game game_check_and_set_victory(Game g) {
  if (g.error.type != E_NONE) {
    g.victory = 0;
    return g;
  }

  for (int y = 0; y < g.rows; y++) {
    for (int x = 0; x < g.cols; x++) {
      Cell c = g.grid[y * g.cols + x];
      if (c.value == V_NONE) {
	g.victory = 0;
	return g;
      }
    }
  }

  g.victory = 1;  
  return g;
}

void game_close(Game g) { free(g.grid); }

// ---------------------------------------------------------------------------

CellValue char_to_value(char c) {
  if (c < '0' || c > '9') {
    return V_INVALID;
  } else {
    return (CellValue)c - '0';
  }
}

Pos coords_to_pos(Vector2 vPos) {
  Pos pos = { 0 };
  pos.x = floorf(vPos.x / CELL_WIDTH);
  pos.y = floorf(vPos.y / CELL_HEIGHT);  
  return pos;
}

int positive_mod(int a, int n) {
  return ((a % n) + n) % n;
}

Pos next_pos(Game g, Dir d) {
  Pos oldPos = g.select;
  Pos newPos = { 0 };

  switch(d) {
  case D_NONE: {
    newPos = oldPos;
    break;
  }
  case D_UP: {
    newPos.x = oldPos.x;
    newPos.y = positive_mod(oldPos.y - 1, g.rows);
    break;
  }
  case D_DOWN: {
    newPos.x = oldPos.x;
    newPos.y = positive_mod(oldPos.y + 1, g.rows);
    break;
  }
  case D_LEFT: {
    newPos.x = positive_mod(oldPos.x - 1, g.cols);
    newPos.y = oldPos.y;
    break;
  }
  case D_RIGHT: {
    newPos.x = positive_mod(oldPos.x + 1, g.cols);
    newPos.y = oldPos.y;    
    break;
  }
  default:
    assert(0 && "Unreachable\n");    
  }
  return newPos;  
}

// ---------------------------------------------------------------------------

void grind_render_lines(Game g) {
  size_t thick_bold = 5;
  size_t thick_light = 1;
  
  // horizontal lines (moving through rows)
  for(size_t y = 1; y < g.rows; y++) {
    Vector2 startPos = { 0, y * CELL_HEIGHT };
    Vector2 endPos = { SCREEN_WIDTH, y * CELL_HEIGHT };
    size_t thickness = y % 3 == 0 ? thick_bold : thick_light;
    DrawLineEx(startPos, endPos, thickness, BLACK);
  }

  // vertical lines (moving through cols)
  for(size_t x = 1; x < g.cols; x++) {
    Vector2 startPos = { x * CELL_WIDTH, 0 };
    Vector2 endPos = { x * CELL_WIDTH , SCREEN_HEIGHT };
    size_t thickness = x % 3 == 0 ? thick_bold : thick_light;
    DrawLineEx(startPos, endPos, thickness, BLACK);
  }
}

void game_render_values(Game g) {
  for (size_t y = 0; y < g.rows; y++) {
    for (size_t x = 0; x < g.cols; x++) {
      Cell c = g.grid[y* g.cols + x];      
      CellValue val = c.value;      
      // CellType type = c.type;

      assert(val != V_INVALID && "Invalid value found in grid");

      if (val != V_NONE) {
	char text[25];
	sprintf(text, "%u", val);
	
	size_t posX = floorf(x * CELL_WIDTH + 0.35f*CELL_WIDTH);
	size_t posY = floorf(y * CELL_HEIGHT + 0.30f*CELL_HEIGHT);
	size_t fontSize = 50;

	DrawText(text, posX, posY, fontSize, BLACK);
      }
    }  
  }
}

void game_render_error(Game g) {
  size_t posX, posY;
  switch(g.error.type) {
  case E_NONE:
    break;

  case E_ROW: {
    posX = 0;
    posY = g.error.index * CELL_HEIGHT;
    DrawRectangle(posX, posY, SCREEN_WIDTH, CELL_HEIGHT, RED);
    break;
  }

  case E_COL: {
    posX = g.error.index * CELL_WIDTH;    
    posY = 0;    
    DrawRectangle(posX, posY, CELL_WIDTH, SCREEN_HEIGHT, RED);    
    break;
  }

  case E_SQUARE: {
    posX = (g.error.index % 3) * 3 * CELL_WIDTH;
    posY = floorf(g.error.index / 3) * 3 * CELL_HEIGHT;
    DrawRectangle(posX, posY, CELL_WIDTH*3, CELL_HEIGHT*3, RED);        
    break;
  }        

  default:
    assert(0 && "Unreachable\n");
    break;
  }  
}

void game_render(Game g) {

  if (IS_OVER(g)) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
  } else if (HAS_SELECTED_CELL(g)) {
    size_t posX = g.select.x * CELL_WIDTH;
    size_t posY = g.select.y * CELL_HEIGHT;
    DrawRectangle(posX, posY, CELL_WIDTH, CELL_HEIGHT, LIGHTGRAY);
  }

  game_render_error(g);
  grind_render_lines(g);
  game_render_values(g);
}

// ---------------------------------------------------------------------------

int main(void) {   
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "sudoku");
  SetWindowPosition(2300, 50);
  SetTargetFPS(60);

  Game g = game_init_from_file(GRID_FILENAME);

  while (!WindowShouldClose()) {
    // STATE MANAGEMENT
    // ------------------------------------------------------
    
    // select a cell with mouse
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 mousePos = GetMousePosition();
      Pos pos = coords_to_pos(mousePos);
      g = game_check_and_set_selection(g, pos);
    }

    // update selected cell with keyboard
    if (HAS_SELECTED_CELL(g)) {
      Dir d = D_NONE;

      if (IsKeyPressed(KEY_UP)) {
	d = D_UP;
      } else if (IsKeyPressed(KEY_DOWN)) {
	d = D_DOWN;
      } else if (IsKeyPressed(KEY_LEFT)) {
	d = D_LEFT;
      } else if (IsKeyPressed(KEY_RIGHT)) {
	d = D_RIGHT;
      }

      Pos newPos = next_pos(g, d);
      g = game_check_and_set_selection(g, newPos);
    }

    // write a number
    // NOTE: careful about convertion unicode -> ascii
    char keyPress = GetCharPressed();
    CellValue val = char_to_value(keyPress);
    g = game_check_and_set_value(g, (Cell){val, DYNAMIC});
    g = game_check_and_set_error(g);
    g = game_check_and_set_victory(g);
    
    // RENDERING
    // ------------------------------------------------------    

    BeginDrawing();
    ClearBackground(WHITE);   
    game_render(g);
    EndDrawing();    
  }

  CloseWindow();
  game_close(g);
  
  return 0;
}
