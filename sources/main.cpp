#include <iostream>
#include <ncurses/curses.h>
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

	WINDOW* title_win = newwin(40, 150, LINES/2-20, COLS/2-75);
	WINDOW* menu_win = newwin(38,148, LINES/2-19, COLS/2-74);
	box(title_win, 0, 0);
	box(menu_win, 0, 0);
	mvwprintw(title_win,0,69,"POMODORO_TUI");
	move(0,0);
	
	wrefresh(title_win);
	wrefresh(menu_win);
	refresh();

	leaveok(stdscr, 1);
	leaveok(menu_win, 1);
	leaveok(title_win, 1);
	
	delwin(title_win);
	delwin(menu_win);

	getch();
	endwin();
	return 0;
}
