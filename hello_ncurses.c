// demo.c
#include <ncurses.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

 initscr();
 noecho();
 curs_set(FALSE);

 mvprintw(0, 0, "Hellow ncurses"); 
 refresh();

 sleep(1);

 endwin(); // Restore normal terminal behavior
}
