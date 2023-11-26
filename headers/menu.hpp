#include <ncurses.h>
#include <string>
#include "Pomodoro.hpp"
#ifndef _MENU_HPP
#define _MENU_HPP
class Menu{
	friend class MenuBar;
	public:
		Menu(std::string, int, std::string*, int); // menu name, hotkey, array of options, n. of options
		void select_next_item(void);							 // a very descriptive name
		void select_previous_item(void);					 // a vert descriptive name
	private:
		int start_x_pos;
		std::string text;
		int trigger;
		std::string* items;
		int num_items;
		int selected_item;
};

class MenuBar{
	public:
		MenuBar(WINDOW* , Menu*, int); /* Specifies in which window exists, what menus is has and how many */ 
		void draw(Pomodoro& );					
		void handle_trigger(int);			 /* handles char given by the user to open specified menu */
		void handle_selection(int, int, Pomodoro&); /* handles choice of option by the user, menu_id + opt_id as parameters*/
		void reset(void);
		void draw_menu(Menu, bool, Pomodoro& );
		void draw_menu_items(Menu);
	private:
		WINDOW* win;
		Menu* menus;
		int num_menus;
		int selected_menu;
		WINDOW* menu_win;
};
#endif
