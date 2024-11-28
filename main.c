#include <stdlib.h>
#include "ripes_system.h"

#define MAX_SNAKE_LENGTH 100
#define MAX_APPLES 10

struct Snake {
  unsigned int x[MAX_SNAKE_LENGTH];
  unsigned int y[MAX_SNAKE_LENGTH];
  unsigned int length;
  unsigned int direction; // 0 = up, 1 = right, 2 = down, 3 = left
};
typedef struct Snake Snake;

struct Apple {
  unsigned int x;
  unsigned int y;
};
typedef struct Apple Apple;
Apple apple_pool[MAX_APPLES];
int apple_pool_index = 0;

unsigned int apple_color = 0x0000ff00; // green
unsigned int snake_color = 0x00ff0000; // red
unsigned int empty_color = 0x00000000; // black
unsigned int snake_head_color = 0x000000ff; // blue


// LED functions
void set_pixel(unsigned int x, unsigned int y, unsigned int color);

// Snake functions
void draw_snake(Snake *snake) ;

// Apple functions
Apple* draw_apple(unsigned int x, unsigned int y);
Apple* generate_apple(unsigned int x, unsigned int y);


// Collision detection
int check_collision_with_apple(Snake *snake, Apple* apple);
int check_collision_with_snake(Snake *snake);

void game_loop() {
  // Game vars
  unsigned int initial_x = 17; // 17 is the middle of the screen, horizontally
  unsigned int initial_y = 12; // 12 is the middle of the screen, vertically
  unsigned int game_state = 1; // 1 = playing, 0 = game over


  // Keyboard vars
  unsigned int *up = D_PAD_0_UP;
  unsigned int *down = D_PAD_0_DOWN;
  unsigned int *left = D_PAD_0_LEFT;
  unsigned int *right = D_PAD_0_RIGHT;
  unsigned int key_up_pressed = 0;
  unsigned int key_down_pressed = 0;
  unsigned int key_left_pressed = 0;
  unsigned int key_right_pressed = 0;


  // Snake vars
  unsigned int snake_length = 4;
  unsigned int x = initial_x;
  unsigned int y = initial_y;
  unsigned int initial_direction = -1; // 0 = up, 1 = down, 2 = left, 3 = right, -1 = no direction (start direction)
  Snake snake;
  snake.length = snake_length;
  snake.direction = initial_direction;

  snake.x[0] = x;
  snake.y[0] = y;
  snake.x[1] = x + 1;
  snake.y[1] = y;
  snake.x[2] = x + 2;
  snake.y[2] = y;
  snake.x[3] = x + 3;
  snake.y[3] = y;


  // Apple vars
  Apple* apple = NULL;
  unsigned int apple_exists = 0;

  // Starts game loop
  while(game_state) {
    // Movement logic
    if (*up == 1 && key_up_pressed == 0){
        y = y < 1 ? 23 : y - 1;
        key_up_pressed = 1;
        snake.direction = 0;
    } else if (*down == 1 && key_down_pressed == 0) {
        y = y >= 23 ? 0 : y + 1;
        key_down_pressed = 1;
        snake.direction = 2;
    } else if (*left == 1 && key_left_pressed == 0) {
        x = x < 1 ? 33 : x - 1;
        key_left_pressed = 1;
        snake.direction = 3;
    } else if (*right == 1 && key_right_pressed == 0) {
        x = x >= 33 ? 0 : x + 1;
        key_right_pressed = 1;
        snake.direction = 1;
    } 
    
    // Debounce keys
    if (*up == 0 && key_up_pressed == 1) {
        key_up_pressed = 0;
    } else if (*down == 0 && key_down_pressed == 1) {
        key_down_pressed = 0;
    } else if (*left == 0 && key_left_pressed == 1) {
        key_left_pressed = 0;
    } else if (*right == 0 && key_right_pressed == 1) {
        key_right_pressed = 0;
    }

    // Apple logic
    if (!apple_exists) {
      unsigned int apple_x = rand() % 34;
      unsigned int apple_y = rand() % 24;
      apple = draw_apple(apple_x, apple_y);
      apple_exists = 1;
    }
    
    // Collision detection
    if (check_collision_with_apple(&snake, apple)) {
      snake.length++;
      apple_exists = 0;
    } else if (check_collision_with_snake(&snake)) {
      printf("Game over\n");
      game_state = 0;
      continue;
    }


    move_snake(&snake);

    snake.direction = -1;
  }

}

/*
  Remove one pixel color, at (x,y)
*/

void unset_pixel(unsigned int x, unsigned int y) {
  unsigned int columns = LED_MATRIX_0_WIDTH;
  unsigned int *led_init_address = LED_MATRIX_0_BASE;
  unsigned int offset;

  offset = (x + (y * columns));
  led_init_address += offset;
  *(led_init_address) = empty_color;
}


/*
  Paint one pixel, at (x,y) with arg color
*/
void set_pixel(unsigned int x, unsigned int y, unsigned int color) {
  unsigned int columns = LED_MATRIX_0_WIDTH;
  unsigned int *led_init_address = LED_MATRIX_0_BASE;
  unsigned int offset;

  // Initial position: (17 + (24 - 12) * 35) = 437
  // (x + (24 - y) * columns);
  offset = (x + (y * columns));
  led_init_address += offset;
  *(led_init_address) = color;
}


/*
  Paint a SNAKE_LENGHT*1 square, representing the snake, with red at (x, y)
*/
void draw_snake(Snake *snake) {
  for (unsigned int i = 0; i < snake->length; i++) {
    if (i == 0) {
      set_pixel(snake->x[i], snake->y[i], snake_head_color);
    } else {
      set_pixel(snake->x[i], snake->y[i], snake_color);
    }
  }
}

void move_snake(Snake* snake) {

  // Snake's starting direction is -1, so it doesn't move until a direction is set
  if(snake->direction == -1) {
    draw_snake(snake);
    return;
  }

  // Clear the last pixel of the snake
  unset_pixel(snake->x[snake->length - 1], snake->y[snake->length - 1]);

  // Move each segment to the position of the previous segment
  for (int i = snake->length - 1; i > 0; i--) {
    snake->x[i] = snake->x[i - 1];
    snake->y[i] = snake->y[i - 1];
  }

  // Update the head position based on the direction
  switch (snake->direction) {
    case 0: // Up
      snake->y[0]--;
      break;
    case 1: // Right
      snake->x[0]++;
      break;
    case 2: // Down
      snake->y[0]++;
      break;
    case 3: // Left
      snake->x[0]--;
      break;
  }

  // Draw the snake at the new position
  draw_snake(snake);
}

// Apple logic

/*
  Generate a new apple at (x, y)
  Disclaimer: This function is a weird workaround to simulate a pool of apples, without malloc
  Returns a pointer to the new apple or returns NULL if apple pool is full
*/
Apple* generate_apple(unsigned int x, unsigned int y) {
  if (apple_pool_index >= MAX_APPLES) {
    printf("Apple pool is full\n");
    return NULL;
  }

  Apple* new_apple = &apple_pool[apple_pool_index++];
  new_apple->x = x;
  new_apple->y = y;
  printf("NewApple at x: %d, y: %d\n", new_apple->x, new_apple->y);


  apple_pool_index++;
  return new_apple;
}

/*
  Draw a 1x1 apple at (x, y)
*/
Apple* draw_apple(unsigned int x, unsigned int y) {
  Apple *apple = generate_apple(x, y);
  if (apple != NULL) {
    set_pixel(apple->x, apple->y, apple_color);
  }
  return apple;
}

int check_collision_with_apple(Snake *snake, Apple* apple) {
  if (snake->x[0] == apple->x && snake->y[0] == apple->y) {
    printf("Collision with apple\n");
    return 1;
  }
  return 0;
}
int check_collision_with_snake(Snake *snake) {
  unsigned int head_x = snake->x[0];
  unsigned int head_y = snake->y[0];

  // Check if the head collides with any of the body segments
  for (unsigned int i = 1; i < snake->length; i++) {
    if (snake->x[i] == head_x && snake->y[i] == head_y) {
      return 1; // Collision detected
    }
  }

  return 0; // No collision
}



/*
  Main function. Starts the game loop
*/
void main() {
  game_loop();
  return 0;
}