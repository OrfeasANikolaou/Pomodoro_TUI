#include "../headers/ncurses_helpers.hpp"
#include <string>
void change_background_color(int color_pair){

	for (int i = 0; i < LINES; ++i){
		move(i, 0);
		chgat(-1, A_NORMAL, color_pair, NULL);
		refresh();
	}

}

void message_no_colors(void){
	std::string msg_1 = "Your terminal does not support ncurses colors.";
	std::string msg_2 = "Please use one that does support them.";
	std::string msg_3 = "Press any key to exit.";
	mvprintw(LINES/2-1, COLS/2-msg_1.length()/2,
					"%s", msg_1.c_str());
	mvprintw(LINES/2, COLS/2-msg_2.length()/2,
					"%s", msg_2.c_str());
	mvprintw(LINES/2+1, COLS/2-msg_3.length()/2,
					"%s", msg_3.c_str());
	getch();
}

void message_no_256(void){
	std::string msg_1 = "Your terminal does not have 256 colors.";
	std::string msg_2 = "Please use one that does have them.";
	std::string msg_3 = "Press any key to exit.";
	mvprintw(LINES/2-1, COLS/2-msg_1.length()/2,
					"%s", msg_1.c_str());
	mvprintw(LINES/2, COLS/2-msg_2.length()/2,
					"%s", msg_2.c_str());
	mvprintw(LINES/2+1, COLS/2-msg_3.length()/2,
					"%s", msg_3.c_str());
	getch();
}


