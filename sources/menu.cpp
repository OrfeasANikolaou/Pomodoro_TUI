#include "../headers/menu.hpp"
#include <ncurses/curses.h>
#include "../headers/enums.h"
#include "../headers/Pomodoro.hpp"
Menu::Menu(std::string txt, int trgr, std::string* itms, int n_items) :
	text(txt), trigger(trgr), items(itms), num_items(n_items), selected_item(0) {}

void Menu::select_next_item(void){
	++this->selected_item;
	if (this->selected_item == this->num_items) this->selected_item = 0;
}
void Menu::select_previous_item(void){
	--this->selected_item;
	if (this->selected_item == -1) this->selected_item = num_items - 1;
}


MenuBar::MenuBar(WINDOW* w, Menu* m, int n) : 
	win(w), menus(m), num_menus(n), selected_menu(-1){
		int curr_pos = 2;
		for (int i = 0; i < this->num_menus; ++i){
			this->menus[i].start_x_pos = curr_pos;
			curr_pos += this->menus[i].text.length() + 1;
		}
		int yMax, xMax, yBeg, xBeg;
		getmaxyx(this->win, yMax, xMax);
		getbegyx(this->win, yBeg, xBeg);
		this->menu_win = newwin(yMax - 2, xMax - 2, yBeg + 1, xBeg + 1);
		keypad(this->menu_win, true);
		wrefresh(this->menu_win);
}

void MenuBar::draw(Pomodoro& pom){
	for (int i = 0; i < this->num_menus; ++i){
		
	refresh();
		this->draw_menu(this->menus[i], this->selected_menu == i, pom);
	}
	this->selected_menu = -1;
}

void MenuBar::handle_trigger(int trigger){
	for (int i = 0; i < this->num_menus; ++i){
		if (trigger == this->menus[i].trigger ||
				trigger - 32 == this->menus[i].trigger){ /* ascii black magic so it works with capital */ 
	 		this->selected_menu = i;							 																	  /* letters too */
		}
	}
}

void MenuBar::handle_selection(int menu_id, int choice_id, Pomodoro& pom){
	switch (menu_id){
		case file:{
			switch (choice_id){ /* selects the choice for FILE menu */
				case new_session:{
					int menuwin_yBeg = getbegy(this->menu_win);
					int menuwin_xBeg = getbegx(this->menu_win);
					int menuwin_yMax = getmaxy(this->menu_win);
					int menuwin_xMax = getmaxx(this->menu_win);
					int new_yBeg = menuwin_yBeg+menuwin_yMax/4;
					int new_xBeg = menuwin_xBeg+menuwin_xMax/4;	
					WINDOW* timer_window = newwin(menuwin_yMax/2+5, menuwin_xMax/2, 
																				new_yBeg-3, new_xBeg);
					werase(this->win);
					wrefresh(this->win);
					box(this->win, 0,0);
					wrefresh(this->win);	
					box(timer_window, 0, 0);
					std::string cntdwn = "COUNTDOWN";
					int timerwin_xMax = getmaxx(timer_window);
					mvwprintw(timer_window, 0, timerwin_xMax/2-cntdwn.length()/2, "%s", cntdwn.c_str());
					wrefresh(timer_window);
					pom.startSession(timer_window);
					break;
				}
				case statistics:{
					mvprintw(0,0,"STATISTICS");
					refresh();
					break;
				}
				case quit:{
					mvprintw(0,0,"QUIT");
					refresh();
					break;
				}	
			}
			break;
		}
		case opts:{
			switch (choice_id){ /* selects the choice for OPTS menu */
				case set_timer:{
					mvprintw(0,0,"SETTING TIMER");
					refresh();
					break;
				}
			}
			break;
		}
		case help:{
			switch (choice_id){ /* selects the choice for HELP menu */
				case about:{
					mvprintw(0,0,"ABOUT");
					refresh();
					break;
				}
			}
			break;
		}
	}	
}

void MenuBar::reset(void){
	for (int i = 0; i < this->num_menus; ++i){
		int temp_x = this->menus[i].start_x_pos;
		std::string temp_txt = this->menus[i].text;
		mvwprintw(this->win, 0, temp_x, "%s", temp_txt.c_str());
	}
	wrefresh(this->win);
}

void MenuBar::draw_menu(Menu menu, bool is_selected, Pomodoro& pom){
	int start_x = menu.start_x_pos;
	std::string text = menu.text;
	if (is_selected) { wattron(this->win, A_STANDOUT); }
	mvwprintw(this->win, 0, start_x, "%s", text.c_str());
	wattroff(this->win, A_STANDOUT);
	wrefresh(this->win);
	int ch;
	draw_menu_items(menu);
	wrefresh(this->menu_win);
	while (is_selected && 
				(ch = wgetch(this->menu_win))){
		switch(ch){
			case KEY_UP: case 'k': case 'K':{
				menu.select_previous_item();
				break; 
			}
			case KEY_DOWN: case 'j': case 'J':{
				menu.select_next_item();
				break;
			}
			case '\n':{ /* case fallthrough is intended */
				/*
				mvprintw(5,5, "selected menu: %d selected item: %d", 
								this->selected_menu, menu.selected_item);
				for (int i = 0; i < this->num_menus; ++i) mvprintw(i,0,"%d", menu.selected_item);
				refresh();
				*/
				this->handle_selection(selected_menu, menu.selected_item, pom);
			}
			default:
				is_selected = false;
				break;
		}
		draw_menu_items(menu);	
	}
	werase(this->menu_win);
	wrefresh(this->menu_win);
	reset();
}

void MenuBar::draw_menu_items(Menu menu){
	int xMax = getmaxx(this->menu_win);
	for (int i = 0; i < menu.num_items; ++i){
		mvwprintw(this->menu_win, i, 0, "%s", menu.items[i].c_str());
		if (menu.selected_item == i) { mvwchgat(this->menu_win, i, 0, xMax, A_NORMAL, 1, NULL); }
		else { mvwchgat(this->menu_win, i, 0, xMax, A_STANDOUT, 1, NULL); }
	}
}
