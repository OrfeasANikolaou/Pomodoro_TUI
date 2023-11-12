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
	init_pair(1, COLOR_WHITE, COLOR_CYAN);
	change_background_color(1);
	/* END INIT */

	WINDOW* main_win = newwin(40, 150, LINES/2-20, COLS/2-75);
	leaveok(main_win, 1);
	move(0,0);
	box(main_win, 0, 0);
	mvwprintw(main_win,0,69,"POMODORO_TUI");
	wrefresh(main_win);


	getch();
	endwin();
	return 0;
}
