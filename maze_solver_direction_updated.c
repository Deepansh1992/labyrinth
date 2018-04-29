#include<ncurses.h>
#include<unistd.h>
#include<time.h>

#define DELAY 30000

int main(int argc, char *argv[]) {
	int x = 1; int y = 1;
	int max_x, max_y, ch;
	//int wmove(WINDOW *win, int y, int x);
	initscr(); 
	noecho(); 
	curs_set(FALSE); 
	keypad(stdscr, TRUE);	
	
	getmaxyx(stdscr, max_y, max_x);	
	//mvprintw(0,0, "value of max_y nad max_x is: %d and %d", max_y, max_x);
	
	refresh(); 
	//sleep(10); 
	while(ch=getch()){
		clear(); 
 		switch(ch){	
			case KEY_LEFT:
				if (x>0 ){				
					mvprintw(y,x,"o");
					--x; 
					break;
					}	
			case KEY_RIGHT:
				if(x<max_x/2){
					mvprintw(y,x,"o");	
					++x;					
					break;
				}
			case KEY_UP:
				if(y>0){
					mvprintw(y,x,"o");
					--y;
					break;
				}
			case KEY_DOWN:
				if (y<max_y/2){
					mvprintw(y,x,"o");	
					++y;	
					break;
				}	
			}	
	}
	endwin(); 
	return 0;
}

