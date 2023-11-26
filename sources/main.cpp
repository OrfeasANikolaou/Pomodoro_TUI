#include <iostream>
#include <ncurses.h>
#include <ncurses/curses.h>
#include <fstream>
#include "../headers//menu.hpp"
#include "../headers/ncurses_helpers.hpp"
#include "../headers/enums.h"
int main(void){
	/* START INIT */
	initscr();
	noecho();		// when typing no characters appear on screen
	cbreak(); // allow for ctrl+c in case of accidental starting new session 
	if (!has_colors()){
		message_no_colors();	
		endwin();
		return -1;
	}
	start_color();														// if no color support terminal then cannot use program
	if( COLORS < 256 || COLOR_PAIRS < 256 ){
		message_no_256();
		endwin();
		return -1;	
	}
	
	init_pair(1, COLOR_WHITE, COLOR_CYAN);
	change_background_color(1);
	/* END INIT */

		WINDOW* title_win = newwin(40, 150, LINES/2-20, COLS/2-75);	//make POMODORO_TUI window
	WINDOW* menu_bar_win = newwin(38,148, LINES/2-19, COLS/2-74); //make menu window file opts help
	box(title_win, 0, 0);																			
	box(menu_bar_win, 0, 0);																			// default boxing
	mvwprintw(title_win,0,69,"POMODORO_TUI");											// give title
	move(0,0);						//basically useless but kept in case if something breaks
		
	wrefresh(title_win);
	wrefresh(menu_bar_win);
	refresh();

	curs_set(0);		//cursor doesn't exist
	

	// menu and options
	std::string menu_file[] = {"NEW SESSION", "STATISTICS", "QUIT", };
	std::string menu_opts[] = {"SET TIMER", };
	std::string menu_help[] = {"ABOUT", };

	Menu menus[3] = {
										Menu("FILE", 'f', menu_file, sizeof(menu_file) / sizeof(std::string)),
										Menu("OPTIONS", 'o', menu_opts, sizeof(menu_opts) / sizeof(std::string)),
										Menu("HELP", 'h', menu_help, sizeof(menu_help) / sizeof(std::string)),
									};
	
	MenuBar menu_bar(menu_bar_win, menus, sizeof(menus) / sizeof(Menu));

	Pomodoro tomato;  // default constructor 25min work 5 min break
	tomato.set_data_from_file("data.txt");	// get totalWorkingTime and totalSessions from data.txt
	menu_bar.draw(tomato);									// draw file options help
	int ch;
	while ( (ch = wgetch(menu_bar_win)) ){	// check for char
		menu_bar.handle_trigger(ch);					// if char =f =o or =h goto specific menu
		menu_bar.draw(tomato);								//draw
	}


	//cleanup
	delwin(title_win);
	delwin(menu_bar_win);

	endwin();
	return 0;
}
