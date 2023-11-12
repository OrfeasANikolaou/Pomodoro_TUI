#include <iostream>
#include "../headers/ncurses_helpers.hpp" // ncurses already included
int main(void){
	/* START INIT */
	initscr();
	noecho();
	if (!has_colors()){
		message_no_colors();
		endwin();
		return -1;
	}
	start_color();
	if( COLORS < 256 || COLOR_PAIRS < 256 ){
		message_no_256();
		endwin();
		return -1;	
	}
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	change_background_color(1);
	/* END INIT */




	getch();
	endwin();
	return 0;
}
