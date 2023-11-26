#include <iostream>
#include <ncurses.h>
#include <ncurses/curses.h>
#include "../headers//menu.hpp"
#include "../headers/ncurses_helpers.hpp" // ncurses already included
enum mfile{new_session, statistics, quit};
enum mopts{set_timer};
enum mhelp{about};
enum m{file, opts, help};
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
	WINDOW* menu_bar_win = newwin(38,148, LINES/2-19, COLS/2-74);
	box(title_win, 0, 0);
	box(menu_bar_win, 0, 0);
	mvwprintw(title_win,0,69,"POMODORO_TUI");
	move(0,0);
	
	wrefresh(title_win);
	wrefresh(menu_bar_win);
	refresh();

	curs_set(0);
	
	std::string menu_file[] = {"NEW SESSION", "STATISTICS", "QUIT", };
	std::string menu_opts[] = {"SET TIMER", };
	std::string menu_help[] = {"ABOUT", };

	Menu menus[3] = {
										Menu("FILE", 'f', menu_file, sizeof(menu_file) / sizeof(std::string)),
										Menu("OPTS", 'o', menu_opts, sizeof(menu_opts) / sizeof(std::string)),
										Menu("HELP", 'h', menu_help, sizeof(menu_help) / sizeof(std::string)),
									};
	
	MenuBar menu_bar(menu_bar_win, menus, sizeof(menus) / sizeof(Menu));
	
	menu_bar.draw();
	int ch;
	while ( (ch = wgetch(menu_bar_win)) ){
		menu_bar.handle_trigger(ch);
		menu_bar.draw();
	}

	delwin(title_win);
	delwin(menu_bar_win);

	getch();
	endwin();
	return 0;
}
