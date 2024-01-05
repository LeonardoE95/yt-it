/*
  This file contains an implementation in C of the famous snake game.

  TODOs:

  - show head
  - show score as text on screen
     https://stackoverflow.com/questions/31365160/sdl-show-score-on-screen
     https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
     https://gigi.nullneuron.net/gigilabs/displaying-text-in-sdl2-with-sdl_ttf/

  Dependencies:
    sudo pacman -S sdl2
    sudo pacman -S sdl2_ttf

 */

// ----------------------------------------------------
// PRE-PROCESSOR STUFF

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#define BOARD_WIDTH 30
#define BOARD_HEIGHT 30

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define CELL_WIDTH ((float) SCREEN_WIDTH / BOARD_WIDTH)
#define CELL_HEIGHT ((float)SCREEN_HEIGHT / BOARD_HEIGHT)

#define MAX_SNAKE_MOVEMENT 0.15   // slowest
#define MIN_SNAKE_MOVEMENT 0.06  // fastest
#define STEP_SNAKE_MOVEMENT 0.03 // step per food eaten

#define DELAY_FOOD_SPAWN 3
#define FOODS_COUNT 1

#define OBSTACLES_COUNT 20

#define MAX_SNAKE_LENGTH ((BOARD_WIDTH) * (BOARD_HEIGHT))

                                     //   R,   G,   B,   A
#define BACKGROUND_COLOR  0x000000FF // 150, 150, 150, 255
#define SNAKE_COLOR       0xEE72F100 // 238, 114, 241, 0
#define FOOD_COLOR        0x77B28C00 // 119, 178, 140, 0
#define OBSTACLE_COLOR    0x964B0000
#define SCORE_COLOR       0xFFFFFF00 // 255, 255, 255, 0

// Thanks Tsoding :D
// https://www.twitch.tv/tsoding
// https://github.com/tsoding/
#define HEX_COLOR(hex)				\
  ((hex) >> (3 * 8)) & 0xFF,			\
  ((hex) >> (2 * 8)) & 0xFF,			\
  ((hex) >> (1 * 8)) & 0xFF,			\
  ((hex) >> (0 * 8)) & 0xFF

// NOTE: to use these off has to be an int initialized to 0
#define STAR_OBSTACLE(game, off, x, y)					\
  (game)->obs[off++] = (Obstacle){x    , y    , 1};			\
  (game)->obs[off++] = (Obstacle){x + 1, y    , 1};			\
  (game)->obs[off++] = (Obstacle){x - 1, y    , 1};			\
  (game)->obs[off++] = (Obstacle){x    , y + 1, 1};			\
  (game)->obs[off++] = (Obstacle){x    , y - 1, 1};

#define HORIZONTAL_WALL_OBSTACLE(game, off, x, y)			\
  (game)->obs[off++] = (Obstacle){x    , y, 1};				\
  (game)->obs[off++] = (Obstacle){x + 1, y, 1};				\
  (game)->obs[off++] = (Obstacle){x + 2, y, 1};				\
  (game)->obs[off++] = (Obstacle){x - 1, y, 1};				\
  (game)->obs[off++] = (Obstacle){x - 2, y, 1};

// ----------------------------------------------------
// STRUCTS & DECLARATIONS

typedef enum {
  DIR_RIGHT = 0,
  DIR_UP,
  DIR_LEFT,
  DIR_DOWN,
} Dir;

// TODO: implement event handling function which is used to execute at
// the right times all sorts of events.
typedef enum {
  AUTOMATIC_MOVEMENT = 0,
  FOOD_SPAWN,
} Event;

typedef struct {
  int x;
  int y;
} Pos;

typedef struct {
  int x;
  int y;
  int init;
} Obstacle;

typedef struct {
  Pos body[MAX_SNAKE_LENGTH];
  int length;
  Dir dir;
} Snake;

typedef struct {
  Pos pos;
  int score;
} Food;

typedef struct {
  Snake snake;
  Food food[FOODS_COUNT];
  Obstacle obs[OBSTACLES_COUNT];
  int global_score;
  double game_speed;
  int quit;
} Game;

int scc(int code);
void *scp(void *ptr);
int random_int_range(int low, int high);
Dir random_dir(void);
Pos random_board_pos(void);
int pos_is_not_empty(Game *game, Pos p);
Pos random_empty_board_pos(Game *game);

// game logic
void init_game(Game *game);
void init_food(Game *game);

Pos *get_snake_head(Snake *snake);
int allow_snake_movement(int manual);
Pos peak_next_pos(Snake *snake, Dir dir);
void move_snake(Game *game, Dir new_dir, int manual);
void print_snake(Game *game);

void update_game_speed(Game *game);

void init_food(Game *game);
int allow_refresh_food(void);
Food *check_for_food(Game *game);
Food *get_food(Pos p);
void update_food(SDL_Renderer *renderer, Game *game);

int check_for_obstacles(Game *game);

// rendering stuff
void render_game(SDL_Renderer *renderer, TTF_Font *font, Game *game);
void render_board(SDL_Renderer *renderer, Game *game);
void render_snake(SDL_Renderer *renderer, Game *game);
void render_food(SDL_Renderer *renderer, Game *game);
void render_obstacles(SDL_Renderer *renderer, Game *game);
void render_game_score(SDL_Renderer *renderer, TTF_Font *font, Game *game);

// ----------------------------------------------------
// GLOBALS

Game GAME = {0};

// ----------------------------------------------------
// UTILS

int scc(int code) {
  if (code < 0) {
    printf("SDL error: %s\n", SDL_GetError());
    exit(1);    
  }

  return code;
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

Dir random_dir(void) {
  return (Dir)random_int_range(0, 4);
}

Pos random_board_pos(void) {
  Pos p = {0};
  p.x = random_int_range(0, BOARD_WIDTH);
  p.y = random_int_range(0, BOARD_HEIGHT);

  return p;
}

int pos_is_not_empty(Game *game, Pos p) {
  // Is there other-food in this spot?
  for (int i = 0; i < FOODS_COUNT; i++)
    if (p.x == game->food[i].pos.x && p.y == game->food[i].pos.y)
      return 1;
  
  // Is there a snake's body parts on this spot?
  for (int i = 0; i < game->snake.length; i++)
    if (p.x == game->snake.body[i].x && p.y == game->snake.body[i].y)
      return 1;

  // is there an obstacle?
  for (int i = 0; i < OBSTACLES_COUNT; i++)
    if (p.x == game->obs[i].x && p.y == game->obs[i].y)
      return 1;
	
  return 0;
}

Pos random_empty_board_pos(Game *game) {
  Pos p = {0};
  do {
    p = random_board_pos();
  } while (pos_is_not_empty(game, p));
  
  return p;
}

// ----------------------------------------------------

// SNAKE LOGIC

Pos *get_snake_head(Snake *snake) {
  return &snake->body[snake->length - 1];
}

Pos peak_next_pos(Snake *snake, Dir dir) {
  Pos new_pos;
  Pos *head_pos = get_snake_head(snake);
  
  switch(dir) {
    // horizontal movements
  case DIR_RIGHT:
    new_pos.x = (head_pos->x + 1) % BOARD_WIDTH;
    new_pos.y = head_pos->y;
    break;
  case DIR_LEFT:
    new_pos.x = head_pos->x == 0 ? BOARD_WIDTH - 1 : head_pos->x - 1;
    new_pos.y = head_pos->y;
    break;
    
    // vertical movements
  case DIR_UP:
    new_pos.y = head_pos->y == 0 ? BOARD_HEIGHT - 1 : head_pos->y - 1;
    new_pos.x = head_pos->x;
    break;
  case DIR_DOWN:
    new_pos.y = (head_pos->y + 1) % BOARD_HEIGHT;
    new_pos.x = head_pos->x;
    break; 
  }

  return new_pos;
}

// Updates the position of the snake based on its latest direction. It
// behaves differently whether or not the user has manually pressed
// any arrow keys. The function calls the sub-procedure
// allow_movement() to determine if the movement to be done is valid
// and, in case of automatic movement, if enough time has passed since
// the last automatic movement.
void move_snake(Game *game, Dir new_dir, int manual) {
  // -- check if movement is valid
  if(!allow_snake_movement(manual)) {
    return;
  }

  Snake *snake = &game->snake;
  Pos new_pos = peak_next_pos(snake, new_dir);

  // -- cant move back to snake's own tail
  if (snake->length >= 2 &&
      new_pos.x == snake->body[snake->length - 2].x &&
      new_pos.y == snake->body[snake->length - 2].y) {
    return;
  }

  // -- perform movement
  Pos *head_pos = get_snake_head(snake);
  Pos old_pos = *head_pos;
  Pos tmp_pos = old_pos;  

  *head_pos = new_pos;
  snake->dir = new_dir;

  for (int i = snake->length - 2; i >= 0; i--) {
    tmp_pos = snake->body[i];
    snake->body[i] = old_pos;
    old_pos = tmp_pos;
  }
}

// Implements the food eating mechanic
Food *check_for_food(Game *game) {
  Snake *snake = &game->snake;
  
  Pos head_pos = *get_snake_head(snake);
  Food *f = get_food(head_pos);

  if (!f) {
    // no food at the head of the snake
    return NULL;
  }

  // ok we have something
  return f;
}

void eat_food(Game *game, Food *f) {
  Snake *snake = &game->snake;
  
  // ok we have a food, let us eat it :D
  GAME.global_score += f->score;

  // next time this food wont be picked up by check_food_presence()
  f->score = 0;  

  // grow snake's body
  Pos next_pos = peak_next_pos(snake, snake->dir);
  snake->length += 1;
  snake->body[snake->length - 1] = next_pos;
}

void print_snake(Game *game) {
  Snake *snake = &game->snake;
  
  printf("snake body { \n");
  printf("\t");
  for (int i = 0; i < snake->length; i++) {
    printf("(%d, %d), ", snake->body[i].x, snake->body[i].y);
  }
  printf("\n\t");
  printf("}\n");
}

// the speed is computed depending on the snake score, but the fastest
// velocity is capped at 0.06.
void update_game_speed(Game *game) {
  double step_update = game->global_score * STEP_SNAKE_MOVEMENT;

  if (game->game_speed - step_update < MIN_SNAKE_MOVEMENT) {
    game->game_speed = MIN_SNAKE_MOVEMENT;
  } else {
    game->game_speed = MAX_SNAKE_MOVEMENT - step_update;
  }

  return;
}

// If the maual parameter is 1 then the user has directly pressed some
// movement keys, like usual arrow keys, and thus the movement is
// always allowed.
//
// If instead the manual parameter is 0, then it means that the call
// was made by the main game loop, and thus the movement is only
// allowed if a certain time-window has passed. THe time window is
// controlled by the variable DELAY.
int allow_snake_movement(int manual) {
  static struct timeval old_t = {0};
  static struct timeval new_t = {0};
  static int init = -1;
  double time_elapsed = -1;

  if (init == -1) {
    // -- first call to function
    init = 1;
    gettimeofday(&old_t, NULL);

    return manual ? 1 : 0;
  }
    
  gettimeofday(&new_t, NULL);
  time_elapsed = (double) (new_t.tv_usec - old_t.tv_usec) / 1000000 +
    (double) (new_t.tv_sec - old_t.tv_sec);
  
  if (!manual && time_elapsed < GAME.game_speed) {
    // not enough time has passed for automatic movement
    return 0;
  } else {
    old_t = new_t;
    return 1;
  }
}

Food *get_food(Pos p) {
  for (int i = 0; i < FOODS_COUNT; i++) {
    Food *f = &GAME.food[i];

    // NOTE: in init_food() we made sure there can only be a single
    // food in any cell, thus matching (x, y) coords determines
    // uniques of food.
    //
    // NOTE: after we eat we set the relative food score to 0 so next
    // time wont be picked up even if the current food window has yet
    // to finish.
    if(f->pos.x == p.x && f->pos.y == p.y && f->score > 0)
      return f;
  }

  return NULL;
}

void remove_food(SDL_Renderer *renderer) {
  scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(BACKGROUND_COLOR)));
  
  for (int i = 0; i < FOODS_COUNT; i++) {
    Food f = GAME.food[i];
    
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

int allow_refresh_food(void) {
  static struct timeval old_t = {0};
  static struct timeval new_t = {0};
  static int init = -1;
  Uint32 time_elapsed = -1;

  if (init == -1) {
    // -- first call to function
    init = 1;
    gettimeofday(&old_t, NULL);
    return 1;
  }
    
  gettimeofday(&new_t, NULL);
  time_elapsed = (double) (new_t.tv_usec - old_t.tv_usec) / 1000000 +
    (double) (new_t.tv_sec - old_t.tv_sec);

  if (time_elapsed < DELAY_FOOD_SPAWN) {
    // not enough time has passed for automatic movement
    return 0;
  } else {
    old_t = new_t;
    return 1;
  }
  
}

void init_food(Game *game) {
  // make sure food can spawn only in cells that are currently free.
  for (int i = 0; i < FOODS_COUNT; i++) {
    game->food[i].score = 1;
    game->food[i].pos = random_empty_board_pos(game);
  }
  
  return;
}

void update_food(SDL_Renderer *renderer, Game *game) {
  // only update after a certain time-window
  if (allow_refresh_food()) {
    remove_food(renderer);
    init_food(game);
  } 
  return;
}

int check_for_obstacles(Game *game) {
  Snake *s = &game->snake;
  Pos head_pos = *get_snake_head(s);

  // did we go into our own tail?
  for(int i = 0; i < s->length - 2; i++)
    if (head_pos.x == s->body[i].x && head_pos.y == s->body[i].y)
      return 1;

  // or against an initialized obstacle?
  for(int i = 0; i < OBSTACLES_COUNT; i++) {
    Obstacle ob = game->obs[i];
    if (ob.init && ob.x == head_pos.x && ob.y == head_pos.y) {
      return 1;
    }
  }

  return 0;
}

void init_game(Game *game) {
  // -- init snake
  game->snake.body[0] = random_board_pos();
  game->snake.length = 1;
  game->snake.dir = random_dir();

  // init food
  init_food(game);

  // init obstacles
  // NOTE: for now these are hard-coded
  int off = 0;
  
  STAR_OBSTACLE(game, off, 7, 10);
  STAR_OBSTACLE(game, off, 23, 10);
  HORIZONTAL_WALL_OBSTACLE(game, off, 15, 20);

  // -- init global state of game 
  game->global_score = 0;
  game->game_speed = MAX_SNAKE_MOVEMENT;
}

// ----------------------------------------------------
// Rendering stuff

void render_game(SDL_Renderer *renderer, TTF_Font *font, Game *game) {
  scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(BACKGROUND_COLOR)));
  scc(SDL_RenderClear(renderer));
  
  // render_board(renderer, game);
  render_snake(renderer, game);
  render_food(renderer, game);
  render_obstacles(renderer, game);
  render_game_score(renderer, font, game);

  SDL_RenderPresent(renderer);
}

void render_board(SDL_Renderer *renderer, Game *game) {
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

void render_snake(SDL_Renderer *renderer, Game *game) {
  Snake *snake = &game->snake;
  
  scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(SNAKE_COLOR)));
  
  // Treat head differently
  SDL_Rect rect = {
    (int) floorf(snake->body[snake->length - 1].x * CELL_WIDTH),
    (int) floorf(snake->body[snake->length - 1].y * CELL_HEIGHT),
    (int) floorf(CELL_WIDTH),
    (int) floorf(CELL_HEIGHT),
  };

  scc(SDL_RenderFillRect(renderer, &rect));

  // NOTE: note head somehow
  // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  // SDL_RenderDrawPoint(renderer, rect.x + rect.w / 2, rect.y + rect.h / 2);
  // SDL_RenderDrawPoint(renderer, rect.x + rect.w / 2, rect.y + 1 + rect.h / 2);

  // rest of body
  for (int i = snake->length - 2; i >=0 ; i--) {
    SDL_Rect rect = {
      (int) floorf(snake->body[i].x * CELL_WIDTH),
      (int) floorf(snake->body[i].y * CELL_HEIGHT),
      (int) floorf(CELL_WIDTH),
      (int) floorf(CELL_HEIGHT),
    };

    scc(SDL_RenderFillRect(renderer, &rect));    
  }

  return;
}

void render_food(SDL_Renderer *renderer, Game *game) {
  scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(FOOD_COLOR)));
  
  for (int i = 0; i < FOODS_COUNT; i++) {
    Food f = game->food[i];

    // only draw food that have not yet been eaten.
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

void render_obstacles(SDL_Renderer *renderer, Game *game) {
  scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(OBSTACLE_COLOR)));

  for (int i = 0; i < OBSTACLES_COUNT; i++) {
    Obstacle ob = game->obs[i];

    if (ob.x == 0 && ob.y == 0)
      continue;
    
    SDL_Rect rect = {
      (int) floorf(ob.x * CELL_WIDTH),
      (int) floorf(ob.y * CELL_HEIGHT),
      (int) floorf(CELL_WIDTH),
      (int) floorf(CELL_HEIGHT),
    };

    scc(SDL_RenderFillRect(renderer, &rect));
  }
}

void render_game_score(SDL_Renderer *renderer, TTF_Font *font, Game *game) {
  static SDL_Surface *surface;
  static SDL_Texture *texture;
  static int init = -1;
  static int prev_score = -1;

  if (prev_score == game->global_score) {
    // -- nothing to update
    return;
  }

  if (init != -1) {
    // -- clean previous allocations
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
  }

  // NOTE: the int shouldn't be longer than 32 characters
  char str[32];
  sprintf(str, "Score: %d", game->global_score);
  
  // -- allocate new stuff
  surface = scp(TTF_RenderText_Solid(font,
				     str,
				     // "Score: 50"
				     (SDL_Color){HEX_COLOR(SCORE_COLOR)}));
  texture = scp(SDL_CreateTextureFromSurface(renderer, surface));

  int textW = 0;
  int textH = 0;
  SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
  SDL_Rect text_rect = {0, 0, textW, textH};  

  SDL_RenderCopy(renderer, texture, NULL, &text_rect);
}

// ---------------------------

int main(int argc, char **argv) {
  // ------------------------------
  // Initialization begin

  // to randomize generation of positions
  srand(time(0));  
  
  // SDL stuff
  scc(SDL_Init(SDL_INIT_VIDEO));
  SDL_Window *const window = scp(SDL_CreateWindow("Description", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));
  SDL_Renderer *const renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
  scc(SDL_RenderSetLogicalSize(renderer, (int) SCREEN_WIDTH, (int) SCREEN_HEIGHT));

  // font stuff
  TTF_Init();
  TTF_Font *font = TTF_OpenFont("../fonts/LiberationMono-Regular.ttf", 30);

  init_game(&GAME);

  // Initialization end
  // ------------------------------  
  
  while (!GAME.quit) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {

      if (event.type == SDL_QUIT) {
	GAME.quit = 1;
	break;
      }
  
      if (event.type == SDL_KEYDOWN) {
	switch(event.key.keysym.sym) {
	case SDLK_UP: {
	  move_snake(&GAME, DIR_UP, 1);
	} break;

	case SDLK_DOWN: {
	  move_snake(&GAME, DIR_DOWN, 1);
	} break;

	case SDLK_LEFT: {
	  move_snake(&GAME, DIR_LEFT, 1);
	} break;

	case SDLK_RIGHT: {
	  move_snake(&GAME, DIR_RIGHT, 1);
	} break;
	}
      }
    }
    
    // main logic loop
    move_snake(&GAME, GAME.snake.dir, 0);
    GAME.quit |= check_for_obstacles(&GAME);
    Food *f = check_for_food(&GAME);
    if (f) {
      eat_food(&GAME, f);
      update_game_speed(&GAME);
    }
    update_food(renderer, &GAME);

    render_game(renderer, font, &GAME);
  }
  
  // Don't forget to free your resources
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
  
  return 0;
}
