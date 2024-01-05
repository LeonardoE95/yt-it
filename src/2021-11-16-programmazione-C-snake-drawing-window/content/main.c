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

// -------------------

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

// -------------------

int main(void) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *const window = scp(SDL_CreateWindow("Description", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));  
  SDL_Renderer *const renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

  int quit = 0;

  while(!quit) {
    SDL_Event event;

    // event handling
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	quit = 1;
      }
    }

    SDL_RenderClear(renderer);
    
    // update game/renderer
    scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0));

    SDL_RenderPresent(renderer);
  }
  
  return 0;
} 
