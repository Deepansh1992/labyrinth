#include<ncurses.h>
#include<unistd.h>

#define DELAY 30000

int main(int argc, char *argv[]) {
	int x = 0; int y = 0;

	initscr(); 
	noecho(); 
	curs_set(FALSE); 

	
	while(1) {
		clear(); 
		mvprintw(y,x,"o"); // to print in x,y coordinate;
		refresh(); 		  //refresh the window 
		
		
		usleep(DELAY); 	  //to give delay 
		x++; 			  // the object "o" in our case will move in 							  // horizontal diection (along the x-axis)
	} 
		
		endwin(); 		  // to close the window ones done 
}

