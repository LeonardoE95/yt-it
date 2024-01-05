/*
  Author: Leonardo Tamiano
  Date: 2022-02-10
  
  To compile execute:

     gcc -lncurses tris.c -o tris
 */

#include <stdio.h>
#include <ncurses.h>
#include <assert.h>
#include <stdbool.h>

#define BOARD_WIDTH 6
#define BOARD_HEIGHT 6

#define START_X_BOARD 3
#define START_Y_BOARD 3

#define PLAYER_O 0
#define PLAYER_X 1

#define PLAYER_ICON(x) ((x) == PLAYER_O) ? "O" : "X"

// (x, y), where x is row and y is column
int BOARD[BOARD_HEIGHT][BOARD_WIDTH];

// ----------------------------------------

void check_pos(int y, int x) {
  assert(x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT && "invalid pos");
}

void board_init(int board[BOARD_HEIGHT][BOARD_WIDTH]) {
  for (int y = 0; y < BOARD_HEIGHT; y++) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
      BOARD[y][x] = -1;
    }
  }
}

// updates the state of the board by moving the player into the position (y, x)
bool board_update(int board[BOARD_HEIGHT][BOARD_WIDTH], int y, int x, int player) {
  check_pos(y, x);  

  bool valid_update = true;
  
  // Can only update board position if it is empty.
  if (board[y][x] != -1) {
    valid_update = false;
  } else {
    board[y][x] = player;
  }

  return valid_update;
}

char* board_get_value(int board[BOARD_HEIGHT][BOARD_WIDTH], int y, int x) {
  check_pos(y, x);
  return board[y][x] == -1 ? " " : PLAYER_ICON(board[y][x]);
}

bool board_check_victory(int board[BOARD_HEIGHT][BOARD_WIDTH], int last_y, int last_x) {
  int val = board[last_y][last_x];

  int y = 0;
  int x = 0;
  
  // check column
  for (y = 0; (y < BOARD_HEIGHT) && (board[y][last_x] == val); y++) {};
  if (y == BOARD_HEIGHT) { return true; }

  // check row
  for (x = 0; (x < BOARD_WIDTH) && (board[last_y][x] == val); x++) {};
  if (x == BOARD_WIDTH) { return true; }

  if (BOARD_HEIGHT == BOARD_WIDTH) {
    // check diagonals (if we have them)
    y = 0;
    x = 0;
    for (; (x < BOARD_WIDTH) && (y < BOARD_HEIGHT) && (board[y][x] == val); x++, y++) {};
    if ((x == BOARD_WIDTH) && (y == BOARD_HEIGHT)) { return true; }

    y = BOARD_HEIGHT - 1;
    x = 0;
    for (; (x < BOARD_WIDTH) && (y > -1) && (board[y][x] == val); x++, y--) {};
    if ((x == BOARD_WIDTH) && (y == -1)) { return true; }
  }

  return false;
}

// ----------------------------------------

void board_write_to_screen(int board[BOARD_HEIGHT][BOARD_WIDTH], int start_y, int start_x) {
  mvprintw(start_y, start_x, "Board status: ");
  
  int col = start_y;
  for (int y = 0; y < BOARD_HEIGHT; y++, col++) {

    move(col, start_x);
    for (int x = 0; x < BOARD_WIDTH; x++) {
      char* board_value = board_get_value(board, y, x);
      char *final_separator = x == BOARD_WIDTH - 1 ? "|" : "\0";

      printw("|%s%s", board_value, final_separator);
    }
    
    printw("\n");
  }
}

void player_write_to_screen(int current_player, int start_y, int start_x, int board_height, int board_width) {
  mvprintw(start_y + board_height, start_x, "\nCurrent player: %s", PLAYER_ICON(current_player));
  printw("\n");
}


// ----------------------------------------

int shift_coord(int coord, int start_coord, bool horiz_mode) {
  return horiz_mode ? (coord - start_coord) / 2: coord - start_coord;
}

bool allow_event(MEVENT *event, int start_y, int start_x) {
  // x is out of bounds
  if (event->x < start_x || event->x > start_x + 2 * BOARD_WIDTH)  {
    return false;
  }

  // y is out of bounds
  if (event->y < start_y || event->y > start_y + (1 * BOARD_HEIGHT - 1))  {
    return false;
  }

  // pos is on board internal boundaries
  if (((event->x - start_x) % 2) == 0) {
    return false;
  }

  return true;  
}

// ----------------------------------------

int main() {
  // init ncurses stuff
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL); // Get all the mouse events
  
  // my init stuff
  board_init(BOARD);
  int current_player = PLAYER_X;
  bool quit = false;
  int count = 0;

  while (!quit) {
    // render board
    board_write_to_screen(BOARD, START_Y_BOARD, START_X_BOARD);
    player_write_to_screen(current_player, START_Y_BOARD, START_X_BOARD, BOARD_HEIGHT, BOARD_WIDTH);
    refresh();

    MEVENT event;
    int ch = getch();
    if (ch == KEY_MOUSE) {
      if(getmouse(&event) == OK) {

	// skip invalid events
	if (!allow_event(&event, START_Y_BOARD, START_X_BOARD))  {
	  // mvprintw(11, 2, "Invalid mouse event!");
	  continue; 
	}

	int x = shift_coord(event.x, START_X_BOARD, true);
	int y = shift_coord(event.y, START_Y_BOARD, false);

	// for debugging
	// mvprintw(10, 2, "[%d] (%d, %d) -> (%d, %d)", current_player, event.y, event.x, y, x);
	// mvprintw(11, 2, "Valid mouse event!");	
	
	bool valid_move = board_update(BOARD, y, x, current_player);
	if (!valid_move) {  continue; } // skip invalid moves

	// check end-game conditions
	bool victory = board_check_victory(BOARD, y, x);
	bool tie = !victory && count == BOARD_WIDTH * BOARD_HEIGHT -1 ? true :  false;
	
	if (victory) {
	  mvprintw(START_Y_BOARD + BOARD_HEIGHT + 5, START_X_BOARD + 2, "Player: %s has won!\n", PLAYER_ICON(current_player));	  
	} else if(tie) {
	  mvprintw(START_Y_BOARD + BOARD_HEIGHT + 5, START_X_BOARD + 2, "Game is tied, gg wp!");
	}
	
	// update for next iteration
        quit = victory | tie;
	current_player = current_player == PLAYER_X ? PLAYER_O : PLAYER_X;	
	count += 1;
      }
    }
  }

  // render one last time
  board_write_to_screen(BOARD, START_Y_BOARD, START_X_BOARD);
  refresh();
  
  getch();  
  endwin();
  
  return 0;
}
