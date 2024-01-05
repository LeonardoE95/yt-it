/*
  Dipendenze:

  - SDL2

    sudo pacman -S sdl2 (archlinux)
    sudo apt-get install libsdl2-dev (ubuntu)

  - SDL2_font

    sudo pacman -S sdl2_ttf (archlinux)
    sudo apt-get install libsdl2-ttf-dev (ubuntu)

 */

#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH  900
#define SCREEN_HEIGHT 900

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20

#define CELL_WIDTH ((float) SCREEN_WIDTH / BOARD_WIDTH)
#define CELL_HEIGHT ((float)SCREEN_HEIGHT / BOARD_HEIGHT)

// ----------------------------------------------------

typedef struct {
  int quit;
} Game;

// -- utils
void scc(int code);
void *scp(void *ptr);

// -- game logic
void init_game(Game *game);

// -- rendering stuff
void render_game(SDL_Renderer *renderer, Game *game);
void render_board(SDL_Renderer *renderer);

// ----------------------------------------------------
// GLOBALS

Game GAME = {0};

// ----------------------------------------------------
// UTILS

// Thanks Tsoding :D
// https://www.twitch.tv/tsoding
// https://github.com/tsoding/
void scc(int code) {
  if (code < 0) {
    printf("SDL error: %s\n", SDL_GetError());
    exit(1);
  }

  return;
}

void *scp(void *ptr) {
  if (ptr == NULL) {
    printf("SDL error: %s\n", SDL_GetError());
    exit(1);
  }

  return ptr;
}


// ----------------------------------------------------
// GAME LOGIC

void init_game(Game *game) {
  game->quit = 0; 
}

// -------------------
// RENDER STUFF

void render_game(SDL_Renderer *renderer, Game *game) {
  scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
  scc(SDL_RenderClear(renderer));

  render_board(renderer);

  SDL_RenderPresent(renderer);
}

void render_board(SDL_Renderer *renderer) {
  scc(SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255));
  
  for(int x = 0; x < BOARD_WIDTH; x++) {
    scc(SDL_RenderDrawLine(renderer,
			   x * CELL_WIDTH,
			   0,
			   x * CELL_WIDTH,
			   SCREEN_WIDTH));
  }


  for(int y = 0; y < BOARD_HEIGHT; y++) {
    scc(SDL_RenderDrawLine(renderer,
			   0,
			   y * CELL_HEIGHT,
			   SCREEN_WIDTH,
			   y * CELL_HEIGHT));
  }
}

// ----------------------------------------------------

int main(void) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *const window = scp(SDL_CreateWindow("Description", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));  
  SDL_Renderer *const renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

  init_game(&GAME);

  while(!GAME.quit) {
    SDL_Event event;

    // event handling
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	GAME.quit = 1;
      }
    }

    SDL_RenderClear(renderer);
    
    // update game/renderer
    render_game(renderer, &GAME);
  }
  
  return 0;
} 
