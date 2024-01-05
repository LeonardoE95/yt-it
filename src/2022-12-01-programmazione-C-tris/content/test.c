#include <stdio.h>
#include <ncurses.h>
#include <assert.h>
#include <stdbool.h>

#define BOARD_WIDTH 3
#define BOARD_HEIGHT 3

int main() {
  // init ncurses stuff  
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  mousemask(ALL_MOUSE_EVENTS, NULL); // Get all the mouse events
  printw("Hello World\n");
  printw("Hellooo\n");
  mvprintw(3, 5, "Board status: ");
  refresh();
  getch();  
  endwin();
  return 0;
}
