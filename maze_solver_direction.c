#include<ncurses.h>
#include<unistd.h>

#define DELAY 30000

int main(int argc, char *argv[]) {
	int x = 0; int y = 0;
	int ch;
	//int wmove(WINDOW *win, int y, int x);
	initscr(); 
	noecho(); 
	curs_set(FALSE); 
	keypad(stdscr, TRUE);	
	refresh(); 

	while(ch=getch()){
		clear(); 
 		switch(ch){	
			case KEY_LEFT:
				mvprintw(x,y,"o");
				y--; 
				break;
			case KEY_RIGHT:
				mvprintw(x,y,"o");	
				y++;					
				break;
			case KEY_UP:
				mvprintw(x,y,"o");
				x--;
				break;
			case KEY_DOWN:
				mvprintw(x,y,"o");	
				x++;	
				break;
			}	
	}
	endwin(); 
	return 0;
}



