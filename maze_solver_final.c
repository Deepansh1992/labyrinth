#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
pthread_mutex_t mutex;

struct coordinate
{
  int x;
  int y;
};

const struct coordinate UP = { 0, -1 };
const struct coordinate DOWN = { 0, 1 };
const struct coordinate RIGHT = { 1, 0 };
const struct coordinate LEFT = { -1, 0 };

struct coordinate LOOK_AROUND[4] = {
  {1, 0},
  {0, -1},
  {-1, 0},
  {0, 1}
};

struct coordinate player_pos;
struct coordinate goal_pos;

#define GAME_AREA_WIDTH 2*20+1
#define GAME_AREA_HEIGHT 2*10+1

#define OFFSET_X 3
#define OFFSET_Y 3

enum
{ WALL, NOTHING } game_area[GAME_AREA_WIDTH][GAME_AREA_HEIGHT];
bool visited[GAME_AREA_WIDTH][GAME_AREA_HEIGHT];
/* Depth first search algorithm to generate the maze*/
void
DFS (int x, int y)
{				//maze generation using depth-first search
  if (visited[x][y])
    return;
  visited[x][y] = TRUE;
  game_area[x][y] = NOTHING;
  char unvisited_neighbour_counter;
  do
    {
      unvisited_neighbour_counter = 0;
      int i;
      for (i = 0; i < 4; i++)
	{
	  int _x = x + (2 * LOOK_AROUND[i].x);
	  int _y = y + (2 * LOOK_AROUND[i].y);
	  if (GAME_AREA_HEIGHT > _y && _y > 0 && GAME_AREA_WIDTH > _x
	      && _x > 0 && visited[_x][_y] == FALSE)
	    {
	      unvisited_neighbour_counter++;
	    }
	}
      if (unvisited_neighbour_counter > 0)
	{
	  int internal_counter = -1;
	  int j = -1;
	  char choice = rand () % (unvisited_neighbour_counter + 1);
	  do
	    {
	      int _x = x + (2 * LOOK_AROUND[j].x);
	      int _y = y + (2 * LOOK_AROUND[j].y);
	      if (GAME_AREA_HEIGHT > _y && _y > 0 && GAME_AREA_WIDTH > _x
		  && _x > 0 && visited[_x][_y] == FALSE)
		internal_counter++;
	      j++;

	    }
	  while (internal_counter != choice - 1);
	  j--;
	  game_area[x + LOOK_AROUND[j].x][y + LOOK_AROUND[j].y] = NOTHING;
	  DFS (x + (2 * LOOK_AROUND[j].x), y + (2 * LOOK_AROUND[j].y));
	}
    }
  while (unvisited_neighbour_counter > 0);
}
/*Thread controlling the drawing of the maze*/
void* draw_game_area_thread_code (void* param)
{
  pthread_mutex_lock(&mutex);
  mvprintw(1,0, "draw game/n");
  int i, j;
  for (i = 0; i < GAME_AREA_WIDTH; i++)
    {
      for (j = 0; j < GAME_AREA_HEIGHT; j++)
	{
	  move (j + OFFSET_X, i + OFFSET_Y);
	  if (game_area[i][j] == WALL)
	    addch (' ' | A_REVERSE);
	  else
	    addch (' ');
	}
    }
  move (OFFSET_X + 1, OFFSET_Y + 1);
  addch (ACS_BOARD | A_BLINK);
  pthread_mutex_unlock(&mutex);
}
/*Functions to draw player and to move the player using the arrow keys*/
void
draw_player ()
{
  move (player_pos.y + OFFSET_Y, player_pos.x + OFFSET_X);
  addch (ACS_DIAMOND);
}

void
game_move (struct coordinate movement)
{
  if (game_area[player_pos.x + movement.x][player_pos.y + movement.y] ==
      NOTHING)
    {
      mvprintw (player_pos.y + OFFSET_Y, player_pos.x + OFFSET_X, " ");
      player_pos.x += movement.x;
      player_pos.y += movement.y;
      draw_player ();
      //mvprintw(1, 0, "x: %d, y: %d      ", player_pos.x, player_pos.y);    
    }
  move (0, 0);
}

/* generating game area*/
void* gen_game_area_thread_code (void* param)  {
  pthread_mutex_lock(&mutex);
  int i, j;
  for (i = 0; i < GAME_AREA_WIDTH; i++)
    {
      for (j = 0; j < GAME_AREA_HEIGHT; j++)
	{
	  game_area[i][j] = WALL;
	  visited[i][j] = FALSE;
	}
    }
  srand (time (NULL));

  player_pos.x = GAME_AREA_WIDTH - 2;
  player_pos.y = GAME_AREA_HEIGHT - 2;
  goal_pos.x = 1;
  goal_pos.y = 1;
  usleep (1000);
  DFS (1, 1);
  pthread_mutex_unlock(&mutex);
}

/*main game controller*/
void* thread_code(void* param){  
  pthread_mutex_lock(&mutex);
  curs_set (0);
  mvprintw (0, 0, "Labirintus");
     draw_player ();
     refresh ();
      do
	{
	  int c = wgetch (stdscr);
	  switch (c)
	    {
	    case KEY_UP:
	      game_move (UP);
	      break;
	    case KEY_DOWN:
	      game_move (DOWN);
	      break;
	    case KEY_RIGHT:
	      game_move (RIGHT);
	      break;
	    case KEY_LEFT:
	      game_move (LEFT);
	      break;
	    }

	}
      while ((goal_pos.x != player_pos.x) || (goal_pos.y != player_pos.y));
  clrtoeol ();
  refresh ();
  endwin ();
  return 0;

}
/*screen initilization*/
void* screen_init_thread_code(void* param){
	pthread_mutex_lock(&mutex);
	initscr ();
	noecho ();
  cbreak ();
  keypad (stdscr, TRUE);
  refresh ();
  pthread_mutex_unlock(&mutex);
}



int main()  {
	mvprintw(0,0," Labyrinth.\n");
	pthread_t thread, thread_screen_init, thread_gen_game_area, thread_draw_game_area;
	pthread_mutex_init(&mutex,0);
	
	pthread_create(&thread_screen_init, 0,&screen_init_thread_code, 0);
	pthread_create(&thread_gen_game_area, 0,&gen_game_area_thread_code, 0);
	pthread_create(&thread_draw_game_area, 0,&draw_game_area_thread_code, 0);
	pthread_create(&thread, 0,&thread_code, 0);
	
	pthread_join(thread_screen_init,0);
	pthread_join(thread_gen_game_area,0);
	pthread_join(thread_draw_game_area, 0);
	pthread_join(thread,0);
	
	pthread_mutex_destroy(&mutex);	
	return 0;	
}

