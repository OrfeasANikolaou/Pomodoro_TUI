#include <ncurses.h>
#include <string>
#ifndef _MENU_HPP
#define _MENU_HPP
class Menu{
	friend class MenuBar;
	public:
		Menu(std::string, int, std::string*, int); // menu name, hotkey, array of options, n. of options
		void select_next_item(void);
		void select_previous_item(void);
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
		MenuBar(WINDOW* , Menu*, int); // In which window, array of menus, n. of menus
		void draw(void);
		int handle_trigger(int);
		void reset(void);
		void draw_menu(Menu, bool);
		void draw_menu_items(Menu);
	private:
		WINDOW* win;
		Menu* menus;
		int num_menus;
		int selected_menu;
		WINDOW* menu_win;
};
#endif
