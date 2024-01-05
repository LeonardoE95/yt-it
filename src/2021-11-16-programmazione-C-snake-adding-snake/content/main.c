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
#include <time.h>
#include <sys/time.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH  900
#define SCREEN_HEIGHT 900

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

#define CELL_WIDTH ((SCREEN_WIDTH / BOARD_WIDTH))
#define CELL_HEIGHT ((SCREEN_HEIGHT / BOARD_HEIGHT))

#define MAX_SNAKE_MOVEMENT 0.15
#define MIN_SNAKE_MOVEMENT 0.06
#define STEP_SNAKE_MOVEMENT 0.03

#define MAX_SNAKE_LENGTH ((BOARD_WIDTH) * (BOARD_HEIGHT))

#define DELAY_FOOD_SPAWN 3
#define FOODS_COUNT 1

// -------------------
// STRUTTURE DATI

typedef enum {
  DIR_RIGHT = 0,
  DIR_UP,
  DIR_LEFT,
  DIR_DOWN,
} Dir;

typedef struct {
  int x;
  int y;
} Pos;

typedef struct {
  Pos pos;
  int score;
} Food;

typedef struct {
  Pos body[MAX_SNAKE_LENGTH];
  int length;
  Dir dir;
} Snake;

typedef struct {
  Snake snake;
  Food food[FOODS_COUNT];
  double game_speed;
  int quit;
} Game;


// -------------------
// DICHIARAZIONI FUNZIONI

void scc(int code);
void *scp(void *ptr);

int random_int_range(int low, int high);
Pos random_board_pos(void);
// TODO: add snake and obstacle cases
int pos_is_not_empty(Game *game, Pos p);
Pos random_empty_board_pos(Game *game);

void init_game(Game *game);
void init_food(Game *game);
int allow_refresh_food(void);

void render_game(SDL_Renderer *renderer, Game *game);
void render_snake(SDL_Renderer *renderer, Game *game);
void render_food(SDL_Renderer *renderer, Game *game);
void remove_food(SDL_Renderer *renderer, Game *game);
void render_board(SDL_Renderer *renderer);

// -------------------
// VARIABILI GLOBALI

Game GAME = {0};

// -------------------
// UTILS

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

int random_int_range(int low, int high) {
  return (rand() % (high - low)) + low;
}

Pos random_board_pos(void) {
  Pos p = {0};
  p.x = random_int_range(0, BOARD_WIDTH);
  p.y = random_int_range(0, BOARD_HEIGHT);

  return p;
}

Dir random_dir(void) {
  return (Dir) random_int_range(0, 4);
  
}

int pos_is_not_empty(Game *game, Pos p) {
  // Food is here?
  for (int i = 0; i < FOODS_COUNT; i++) {
    if (p.x == game->food[i].pos.x && p.y == game->food[i].pos.y)
      return 1;
  }

  // TODO: Snake is here?
  
  // TODO: Obstacle is here?

  return 0;
}

Pos random_empty_board_pos(Game *game) {
  Pos p = {0};
  do {
    p = random_board_pos();
  } while (pos_is_not_empty(game, p));

  return p;
}

// -------------------
// GAME LOGIC FUNCTIONS

void init_game(Game *game) {
  // -- init snake
  game->snake.body[0] = random_board_pos();
  game->snake.length = 1;
  game->snake.dir = random_dir();
  
  game->quit = 0;

  init_food(game);
}

Pos *get_snake_head(Snake *snake) {
  return &snake->body[snake->length - 1];
}

int allow_snake_movement(int manual) {
  // TODO: check if window has it
  static struct timeval old_t = {0};
  static struct timeval new_t = {0};
  static int init = -1;
  Uint32 time_elapsed = -1;

  if (init == -1) {
    init = 1;
    gettimeofday(&old_t, NULL);
    
    return manual ? 1 : 0;
  }

  gettimeofday(&new_t, NULL);
  time_elapsed = (double) (new_t.tv_usec - old_t.tv_usec) / 1000000 + (double) (new_t.tv_sec - old_t.tv_sec);

  if (!manual && time_elapsed < DELAY_FOOD_SPAWN) {
    return 0;
  } else {
    old_t = new_t;
    return 1;
  }
  
}

Pos peak_next_pos(Snake *snake, Dir new_dir) {
  Pos new_pos;
  Pos *head_pos = get_snake_head(snake);

  switch(new_dir) {
  case DIR_RIGHT:
    new_pos.x = (head_pos->x + 1) % BOARD_WIDTH;
    new_pos.y = head_pos->y;
    break;
    
  case DIR_LEFT:
    new_pos.x = head_pos->x == 0 ? BOARD_WIDTH - 1 : head_pos->x - 1;
    new_pos.y = head_pos->y;
    break;

  case DIR_UP:
    new_pos.x = head_pos->x;    
    new_pos.y = head_pos->y == 0 ? BOARD_HEIGHT - 1 : head_pos->y - 1;
    break;

  case DIR_DOWN:
    new_pos.x = head_pos->x;    
    new_pos.y = (head_pos->y + 1) % BOARD_HEIGHT;
    break;
  }

  return new_pos;
  
}

void move_snake(Game *game, Dir new_dir, int manual) {
  if(!allow_snake_movement(manual)) {
    return;
  }

  Snake *snake = &game->snake;
  Pos new_pos = peak_next_pos(snake, new_dir);

  // cant move back to snake's own tail
  if (snake->length >= 2 &&
      new_pos.x == snake->body[snake->length - 2].x &&
      new_pos.y == snake->body[snake->length - 2].y)
    return;

  // perform movement
  Pos *head_pos = get_snake_head(snake);
  Pos old_pos = *head_pos;
  Pos tmp_pos = old_pos;

  *head_pos = new_pos;
  snake->dir = new_dir;

  for (int i = snake->length -2 ; i >= 0; i--) {
    tmp_pos = snake->body[i];
    snake->body[i] = old_pos;
    old_pos = tmp_pos;
  }
  
}


void init_food(Game *game) {
  for (int i = 0; i < FOODS_COUNT; i++) {
    game->food[i].score = 1;
    game->food[i].pos = random_empty_board_pos(game);
  }

  return;
}

int allow_refresh_food(void) {
  // TODO: check if window has it
  static struct timeval old_t = {0};
  static struct timeval new_t = {0};
  static int init = -1;
  Uint32 time_elapsed = -1;

  if (init == -1) {
    init = 1;
    gettimeofday(&old_t, NULL);
    return 1;
  }

  gettimeofday(&new_t, NULL);
  time_elapsed = (double) (new_t.tv_usec - old_t.tv_usec) / 1000000 + (double) (new_t.tv_sec - old_t.tv_sec);

  if (time_elapsed < DELAY_FOOD_SPAWN) {
    return 0;
  } else {
    old_t = new_t;
    return 1;
  }
}

void update_food(SDL_Renderer *renderer, Game *game) {
  if (allow_refresh_food()) {
    remove_food(renderer, game);
    init_food(game);
  }

  return;
}

// -------------------
// RENDER FUNCTIONS

void render_game(SDL_Renderer *renderer, Game *game) {
  scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0));
  SDL_RenderClear(renderer);
  
  render_board(renderer);
  render_snake(renderer, game);
  render_food(renderer, game);

  SDL_RenderPresent(renderer);
}

void render_board(SDL_Renderer *renderer) {
  scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0));
  
  for(int x = 0; x < BOARD_WIDTH; x++) {
    SDL_RenderDrawLine(renderer,
		       x * CELL_WIDTH,
		       0,
		       x * CELL_WIDTH,
		       SCREEN_HEIGHT
		       );
  }

  for(int y = 0; y < BOARD_HEIGHT; y++) {
    SDL_RenderDrawLine(renderer,
		       0,
		       y * CELL_HEIGHT,			 
		       SCREEN_WIDTH,
		       y * CELL_HEIGHT
		       );
  }
}

void render_snake(SDL_Renderer *renderer, Game *game) {
  scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0));
  
  Snake *snake = &game->snake;

  for (int i = snake->length - 1; i >= 0; i--) {
    Pos p = snake->body[i];
    
    SDL_Rect rect = {
      (int) floorf(p.x * CELL_WIDTH),
      (int) floorf(p.y * CELL_HEIGHT),
      (int) floorf(CELL_WIDTH),
      (int) floorf(CELL_HEIGHT)
    };

    scc(SDL_RenderFillRect(renderer, &rect));
  }

  return;
}

void render_food(SDL_Renderer *renderer, Game *game) {
  scc(SDL_SetRenderDrawColor(renderer, 119, 178, 140, 0));

  for (int i = 0; i < FOODS_COUNT; i++) {
    Food f = game->food[i];

    if (f.score == 0) {
      continue;
    }

    SDL_Rect rect = {
      (int) floorf(f.pos.x * CELL_WIDTH),
      (int) floorf(f.pos.y * CELL_HEIGHT),
      (int) floorf(CELL_WIDTH),
      (int) floorf(CELL_HEIGHT),
    };

    scc(SDL_RenderFillRect(renderer, &rect));
  }
}

void remove_food(SDL_Renderer *renderer, Game *game) {
  scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0));

  for (int i = 0; i < FOODS_COUNT; i++) {
    Food f = game->food[i];

    if (!f.score)
      continue;

    SDL_Rect rect = {
      (int) floorf(f.pos.x * CELL_WIDTH),
      (int) floorf(f.pos.y * CELL_HEIGHT),
      (int) floorf(CELL_WIDTH),
      (int) floorf(CELL_HEIGHT),
    };

    scc(SDL_RenderFillRect(renderer, &rect));

  }
  
}


// -------------------


int main(void) {
  srand(time(0));
  
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

      if (event.type == SDL_KEYDOWN) {
	switch (event.key.keysym.sym) {
	case SDLK_UP: {
	  move_snake(&GAME, DIR_UP, 1);
	  break;
	}

	case SDLK_DOWN: {
	  move_snake(&GAME, DIR_DOWN, 1);
	  break;
	}

	case SDLK_LEFT: {
	  move_snake(&GAME, DIR_LEFT, 1);
	  break;
	}

	case SDLK_RIGHT: {
	  move_snake(&GAME, DIR_RIGHT, 1);
	  break;
	}	  
	}
      }
      
    }

    // main logic loop
    move_snake(&GAME, GAME.snake.dir, 0);
    update_food(renderer, &GAME);

    // rendering stuff
    render_game(renderer, &GAME);

  }
  
  return 0;
}
