#include "../headers/menu.hpp"
#include <ncurses/curses.h>
#include "../headers/enums.h"
#include "../headers/Pomodoro.hpp"
// menu constructor (ie. FILE)
Menu::Menu(std::string txt, int trgr, std::string* itms, int n_items) :
	text(txt), trigger(trgr), items(itms), num_items(n_items), selected_item(0) {}


// if up/down when selecting option from menu 
void Menu::select_next_item(void){
	++this->selected_item;
	if (this->selected_item == this->num_items) this->selected_item = 0;
}
void Menu::select_previous_item(void){
	--this->selected_item;
	if (this->selected_item == -1) this->selected_item = num_items - 1;
}

//Menu bar holds menus and their windows
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


// handle selection of menu-->option
void MenuBar::handle_selection(int menu_id, int choice_id, Pomodoro& pom){
	switch (menu_id){	//which menu
		case file:{
			switch (choice_id){ /* which choice from FILE */
				case new_session: {// cool enums
					int menuwin_yBeg = getbegy(this->menu_win);
					int menuwin_xBeg = getbegx(this->menu_win);
					int menuwin_yMax = getmaxy(this->menu_win);
					int menuwin_xMax = getmaxx(this->menu_win);
					int new_yBeg = menuwin_yBeg+menuwin_yMax/4;	// VERY boring windowing stuff
					int new_xBeg = menuwin_xBeg+menuwin_xMax/4;	
					WINDOW* timer_window = newwin(menuwin_yMax/2+5, menuwin_xMax/2, 
																				new_yBeg-3, new_xBeg);
					werase(this->win);
					wrefresh(this->win);
					box(this->win, 0,0);	// been an hour and a half on this debugging
					wrefresh(this->win);	
					box(timer_window, 0, 0);
					std::string cntdwn = "COUNTDOWN";	// title
					int timerwin_xMax = getmaxx(timer_window);
					mvwprintw(timer_window, 0, timerwin_xMax/2-cntdwn.length()/2, "%s", cntdwn.c_str());
					wrefresh(timer_window);
					pom.startSession(timer_window);	// THE STAR OF THE SHOW
					break;
				}
				case statistics:{
					int menuwin_yBeg = getbegy(this->menu_win);
					int menuwin_xBeg = getbegx(this->menu_win);
					int menuwin_yMax = getmaxy(this->menu_win);
					int menuwin_xMax = getmaxx(this->menu_win);	// more very boring windowig stuff
					int new_yBeg = menuwin_yBeg+menuwin_yMax/4;
					int new_xBeg = menuwin_xBeg+menuwin_xMax/4;	
					WINDOW* stats_window = newwin(menuwin_yMax/2+5, menuwin_xMax/2, 
																				new_yBeg-3, new_xBeg);
					werase(this->win);
					wrefresh(this->win);
					box(this->win, 0,0);
					wrefresh(this->win);	
					box(stats_window, 0, 0);
					std::string sts = "STATISTICS";
					int stat_xMax = getmaxx(stats_window);
					mvwprintw(stats_window, 0, stat_xMax/2-sts.length()/2, "%s", sts.c_str());
					pom.getStatistics(stats_window);	// cool feature but broken :(
					wrefresh(stats_window);
					refresh();
					getch();
					break;
				}
				case quit:{
					endwin();
					exit(0);
					break;
				}	
			break;
			}
		
		case opts:{
			switch (choice_id){ /* selects the choice for OPTS menu */
				case set_timer:{
//					mvprintw(0,0,"SETTING TIMER");
					int menuwin_yBeg = getbegy(this->menu_win);
					int menuwin_xBeg = getbegx(this->menu_win);
					int menuwin_yMax = getmaxy(this->menu_win);
					int menuwin_xMax = getmaxx(this->menu_win);

					WINDOW* settings_window = newwin(menuwin_yMax/2, menuwin_xMax*3/4, 
																				menuwin_yBeg*2, menuwin_xBeg+20);
					werase(this->win);
					wrefresh(this->win);
					box(this->win, 0,0);
					wrefresh(this->win);	
					box(settings_window, 0, 0);
					std::string sttngs = "SET-TIMER";
					// messages for ranges and for what
					std::string msg_1 = "Enter hours for working time (0-23): ";
					std::string msg_2 = "Enter minutes for working time (0-59): ";
					std::string msg_3 = "Enter hours for break time (0-23): ";
					std::string msg_4 = "Enter mintes for break time (0-59): ";
					std::string msg_5 = "Timer is set press any key to continue :)"; // φιλικότητα πρως τον χρήστη
					int settingswin_xMax = getmaxx(settings_window);
					int work_h, work_m, break_h, break_m;
					echo();	// echo so you can see what you type
					
					//title
					mvwprintw(settings_window, 0, settingswin_xMax/2-sttngs.length()/2, "%s", sttngs.c_str());
					

					// hours for working time
					mvwprintw(settings_window, 3, settingswin_xMax/2-msg_1.length(), "%s", msg_1.c_str());	
					do{
						mvwprintw(settings_window, 3, settingswin_xMax/2 + 1, "\t\t\t\t\t\t\t");
						mvwscanw(settings_window, 3, settingswin_xMax/2 + 1, "%d", &work_h);
					}while(work_h < 0 || 23 < work_h);
					
					//minutes for working time
					mvwprintw(settings_window, 6, settingswin_xMax/2-msg_2.length(), "%s", msg_2.c_str());	
					do{
						mvwprintw(settings_window, 6, settingswin_xMax/2 + 1, "\t\t\t\t\t\t\t");
						mvwscanw(settings_window, 6, settingswin_xMax/2 + 1, "%d", &work_m);
					}while(work_m < 0 || 59 < work_m);
					
					// hours for break time
					mvwprintw(settings_window, 9, settingswin_xMax/2-msg_3.length(), "%s", msg_3.c_str());	
					do{
						mvwprintw(settings_window, 9, settingswin_xMax/2 + 1, "\t\t\t\t\t\t\t");
						mvwscanw(settings_window, 9, settingswin_xMax/2 + 1, "%d", &break_h);
					}while(break_h < 0 || 23 < break_h);
					
					// minutes for break time
					mvwprintw(settings_window,12, settingswin_xMax/2-msg_4.length(), "%s", msg_4.c_str());	
					do{
						mvwprintw(settings_window,12, settingswin_xMax/2 + 1, "\t\t\t\t\t\t\t");
						mvwscanw(settings_window,12, settingswin_xMax/2 + 1, "%d", &break_m);
					}while(break_m < 0 || 59 < break_m);
					noecho(); // stop echoing what you type since no longer needed
										

					//friendly message towards the user
					mvwprintw(settings_window, 15, settingswin_xMax/2-msg_5.length()/2, "%s", msg_5.c_str());	
					wrefresh(settings_window);
			
					//setting new config with assignment operator magic
					pom = Pomodoro((work_h * 60) + work_m, (break_h * 60) + break_m);	
					getch();

					//cleanup
					wclear(settings_window);
					wrefresh(settings_window);	
					delwin(settings_window);
					break;
				}
			}
			break;
		}
		case help:{
			switch (choice_id){ /* selects the choice for HELP menu */
				case about:{
										 // it's just a message about AGPLv3 and such nothing important
					werase(this->win);
					wrefresh(this->win);
					box(this->win, 0,0);
					wrefresh(this->win);	
					int yBeg = getbegy(this->win);
					int xBeg = getbegx(this->win);
					int yMax = getmaxy(this->win);
					int xMax = getmaxx(this->win);
					WINDOW* about_window = newwin(yMax/2, xMax/2, yBeg+yMax/4, xBeg+xMax/4);
					box(about_window, 0, 0);
					mvwprintw(about_window, 1, 13, "This software is licensed under the AGPLv3-or-later"); 
					mvwprintw(about_window, 2, 15,	"For more information please visit the FSF website");
					mvwprintw(about_window, 5, 12, "Contact: orfeasnikolaou.dev@gmail.com");
					mvwprintw(about_window, 7, 12, "       : www.github.com/OrfeasANikolaou");
					wrefresh(about_window);
					getch();
					break;
				}
			}
			break;
		}
	}	
	}
}

void MenuBar::reset(void){
	//reset menus to normal color after being highlighted
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
	mvwprintw(this->win, 0, start_x, "%s", text.c_str());  //if menu is selected will be highligted
	wattroff(this->win, A_STANDOUT);
	wrefresh(this->win);
	int ch;

	draw_menu_items(menu);		// draw its items/options/
	wrefresh(this->menu_win);
	while (is_selected && 
				(ch = wgetch(this->menu_win))){
		switch(ch){
			// up key == 'k' == 'K'
			case KEY_UP: case 'k': case 'K':{
				menu.select_previous_item();
				break; 
			}

			// same for down key and j
			case KEY_DOWN: case 'j': case 'J':{
				menu.select_next_item();
				break;
			}
			//handles the selection of menu + choice in above huge switch statement
			case '\n':{ /* case fallthrough is intended */
				this->handle_selection(selected_menu, menu.selected_item, pom);
			}
			// naturally then stops being selected
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
	// iterates through all menu options and does stuff like highlight
	int xMax = getmaxx(this->menu_win);
	for (int i = 0; i < menu.num_items; ++i){
		mvwprintw(this->menu_win, i, 0, "%s", menu.items[i].c_str());
		if (menu.selected_item == i) { mvwchgat(this->menu_win, i, 0, xMax, A_NORMAL, 1, NULL); }
		else { mvwchgat(this->menu_win, i, 0, xMax, A_STANDOUT, 1, NULL); }
	}
}
