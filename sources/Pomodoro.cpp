#include "../headers/Pomodoro.hpp"
#include "../headers/ncurses_helpers.hpp"
#include <chrono>
#include <ncurses.h>
#include <ncurses/curses.h>
#include <thread>
#include <iostream>

void pomodoro_helper_countdown(bool& p, int& time, WINDOW* win, bool flag){
	int xMax, yMax;
	getmaxyx(win, yMax, xMax);
	int hrs, mins, secs;
	std::string msg_2 = "PRESS ANY KEY TO PAUSE AND RESUME";
	std::string msg_3 = "Press any key to continue";
	std::string msg_4 = "Press any key to start break";
	std::string msg_5 = "Do you wish to continue with another Pomodoro session?";
	if (flag) { // work time
		std::string msg_1 = "YOU HAVE TO FOCUS";
		init_pair(3, COLOR_WHITE, COLOR_RED);
		wattron(win,COLOR_PAIR(3));
		mvwprintw(win, yMax/2-8, xMax/2-msg_1.length()/2, "%s", msg_1.c_str());
		wattroff(win,COLOR_PAIR(3));
		wattron(win, A_REVERSE);
		mvwprintw(win, yMax/2+8, xMax/2-msg_2.length()/2, "%s", msg_2.c_str());
		wattroff(win, A_REVERSE);
		wrefresh(win);
		while (time){
			while (p){std::this_thread::sleep_for(std::chrono::seconds(1));}
				hrs  = time / 3600;					
				mins = (time % 3600) / 60; 	 	
				secs = time % 60;						 
				mvwprintw(win, yMax/2-4, xMax/2-13,
									"Time remaining: %02d::%02d::%02d", hrs, mins, secs);
				wrefresh(win);
				--time;
				std::this_thread::sleep_for(std::chrono::seconds(1));			
			
		}
				mvwprintw(win, yMax/2+4, xMax/2-msg_3.length()/2, "%s", msg_3.c_str());
				mvwprintw(win, yMax/2-4, xMax/2-13,
									"Time remaining: %02d::%02d::%02d", hrs, mins, 0);	// ultra mega spaghetti
	}
	else { // break time
		mvwprintw(win, yMax/2+4, xMax/2-msg_3.length()/2, "                                    ");
		std::string msg_1 = "YOU HAVE TO RELAX";
		init_pair(4, COLOR_WHITE, COLOR_GREEN);
		wattron(win, COLOR_PAIR(4));
		mvwprintw(win, yMax/2-8, xMax/2-msg_1.length()/2, "%s", msg_1.c_str());
		wattroff(win,COLOR_PAIR(4));
		wattron(win, A_REVERSE);
		mvwprintw(win, yMax/2+8, xMax/2-msg_2.length()/2, "%s", msg_2.c_str());
		wattroff(win, A_REVERSE);
		mvwprintw(win,  yMax/2+4, xMax/2-msg_4.length()/2, "%s", msg_4.c_str());
		wrefresh(win);
		while (time){
			mvwprintw(win,  yMax/2+4, xMax/2-msg_4.length()/2, "                                    ");
																													//ncurses only loads this once because it's smart
			while (p){std::this_thread::sleep_for(std::chrono::seconds(1));}
				hrs  = time / 3600;					
				mins = (time % 3600) / 60; 	 	
				secs = time % 60;						 
				mvwprintw(win, yMax/2-4, xMax/2-13,
									"Time remaining: %02d::%02d::%02d", hrs, mins, secs);
				wrefresh(win);
				--time;
				std::this_thread::sleep_for(std::chrono::seconds(1));			
			
		}
				mvwprintw(win, yMax/2-4, xMax/2-13,
									"Time remaining: %02d::%02d::%02d", hrs, mins, 0);	// ultra mega spaghetti
		
			mvwprintw(win, yMax/2+3, xMax/2-msg_5.length()/2, "%s", msg_5.c_str());
	}
			mvwprintw(win, yMax/2+8, xMax/2-msg_2.length()/2, "                                       ");
			wrefresh(win);
}
void pomodoro_helper_pause_resume(bool& p, int& time){
	while (time){
		getch();
		p = !p;
	}
}


Pomodoro::Pomodoro() : workDuration(std::chrono::minutes(25)), breakDuration(std::chrono::minutes(5)) {}
Pomodoro::Pomodoro(int wd, int bd){
	using namespace std::chrono;  
if (wd < 1) this->workDuration = minutes(1);
else if (wd > 1440) this->workDuration = minutes(1440);
else this->workDuration = minutes(wd);

if (bd < 1) this->breakDuration = minutes(1);
else if (bd > 1440) this->breakDuration = minutes(1440);
else this->breakDuration = minutes(bd);
}


void Pomodoro::startSession(WINDOW* w){
	bool paused = false;
//  int time =  std::chrono::duration_cast<std::chrono::seconds>(this->workDuration).count(); // spaghetti
	int time = 10;
	std::thread countdown_thread1(pomodoro_helper_countdown, std::ref(paused), std::ref(time), std::ref(w), 1);	
	std::thread pause_resume_thread1(pomodoro_helper_pause_resume, std::ref(paused), std::ref(time));
	while (! countdown_thread1.joinable());
	while (! pause_resume_thread1.joinable());
	countdown_thread1.join();
	pause_resume_thread1.join();

	// time = std::chrono::duration_cast<std::chrono::seconds>(this->breakDuration).count(); // more spaghetti
	time = 10;
	std::thread countdown_thread2(pomodoro_helper_countdown, std::ref(paused), std::ref(time), std::ref(w), 0);	
	std::thread pause_resume_thread2(pomodoro_helper_pause_resume, std::ref(paused), std::ref(time));
	while (! countdown_thread2.joinable());
	while (! pause_resume_thread2.joinable());
	countdown_thread2.join();
	pause_resume_thread2.join();

	
}

void Pomodoro::endSession(void){

}
// πολυ αστειο ονομα επειδη δεν μπορω να βαλω this ως ονομα, αλλα αναφαιρομαι στο παραθυρο για στατιστικα
void Pomodoro::getStatistics(WINDOW* ekeino){
	
}

/* intended for test cases */

std::chrono::minutes Pomodoro::GET_breakDuration(void) const {
	return this->breakDuration;
}
std::chrono::minutes Pomodoro::GET_workDuration(void) const {
	return this->workDuration;
}
