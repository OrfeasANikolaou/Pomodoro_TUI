#include "../headers/Pomodoro.hpp"
#include "../headers/ncurses_helpers.hpp"
#include <chrono>
#include <fstream>
#include <ncurses.h>
#include <thread>
#include <iostream>


// apocalyptic levels of spaghetti and repeated code because of rushing.
void pomodoro_helper_countdown(bool& p, int& time, WINDOW* win, bool flag){
	int xMax, yMax;
	getmaxyx(win, yMax, xMax);
	int hrs, mins, secs;
	std::string msg_2 = "PRESS ANY KEY TO PAUSE AND RESUME";
	std::string msg_3 = "Press any key to continue";
	std::string msg_4 = "Press any key to start break";
	std::string msg_5 = "Do you wish to continue with another Pomodoro session?";
	mvwprintw(win, yMax/2+3, xMax/2-msg_5.length()/2, "                                                        ");
	if (flag) { // work time
		//basically the same as below no need to comment same thing until refactor
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
		// messages
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
			// time remaning
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
				// must show 00:00:00 when timer ends
				mvwprintw(win, yMax/2-4, xMax/2-13,
									"Time remaining: %02d::%02d::%02d", hrs, mins, 0);	// ultra mega spaghetti
		
			mvwprintw(win, yMax/2+3, xMax/2-msg_5.length()/2, "%s", msg_5.c_str());
	}
			mvwprintw(win, yMax/2+8, xMax/2-msg_2.length()/2, "                                       ");
			wrefresh(win);
}
void pomodoro_helper_pause_resume(bool& p, int& time){
	// press any key, it flips whether it's paused or not
	while (time){
		getch();
		p = !p;
	}
}


Pomodoro::Pomodoro() : workDuration(std::chrono::minutes(25)), breakDuration(std::chrono::minutes(5)) {}
Pomodoro::Pomodoro(int wd, int bd){
	using namespace std::chrono;  
	//anything 0 or less gets set to default values 25/5
	//anything too high will be set to a max of 24h
if (wd <= 0) this->workDuration = minutes(25);
else if (wd > 1440) this->workDuration = minutes(1440);
else this->workDuration = minutes(wd);

if (bd <= 0) this->breakDuration = minutes(5);
else if (bd > 1440) this->breakDuration = minutes(1440);
else this->breakDuration = minutes(bd);
}



void Pomodoro::startSession(WINDOW* w){
AGAIN:
	bool paused = false;
  int time =  std::chrono::duration_cast<std::chrono::seconds>(this->workDuration).count(); // spaghetti
//	int time = 5; /* COMMENT ABOVE LINE AND UNCOMMENT THIS ONE FOR TESTING */

	//handles when work time
	std::thread countdown_thread1(pomodoro_helper_countdown, std::ref(paused), std::ref(time), std::ref(w), 1);	
	std::thread pause_resume_thread1(pomodoro_helper_pause_resume, std::ref(paused), std::ref(time));
	while (! countdown_thread1.joinable());
	while (! pause_resume_thread1.joinable());
	countdown_thread1.join();
	pause_resume_thread1.join();
	time = std::chrono::duration_cast<std::chrono::seconds>(this->breakDuration).count(); // more spaghetti
//	time = 5; /* COMMENT ABOVE LINE AND UNCOMMENT THIS ONE FOR TESTING */

	// handles when rest time
	std::thread countdown_thread2(pomodoro_helper_countdown, std::ref(paused), std::ref(time), std::ref(w), 0);	
	std::thread pause_resume_thread2(pomodoro_helper_pause_resume, std::ref(paused), std::ref(time));
	while (! countdown_thread2.joinable());
	while (! pause_resume_thread2.joinable());
	countdown_thread2.join();
	pause_resume_thread2.join();
	this->totalWorkTime += this->workDuration;
	this->sessionsCompleted++;
	this->input_data_to_file("data.txt");	
	if (this->endSession(w)) goto AGAIN;	// if endsession decides to start again goes to top of the function
}

bool Pomodoro::endSession(WINDOW* w){
	// handles whether to continue with another work-rest session or to stop
	bool ret = false;
	std::string choices[] = {"Yes! I'd love to!","Please no...", };
	size_t hl = 0;
	size_t n_choices = sizeof(choices) / sizeof(std::string);
	int yBeg = getbegy(w);
	int xBeg = getbegx(w);
	WINDOW* choice = newwin(4, 23, yBeg*2+2, xBeg*2-xBeg/2-16);
	box(choice, 0, 0);
	while(1){
		for (size_t i = 0; i < n_choices; ++i){
			if (i == hl) { wattron(choice, A_REVERSE); wattron(choice, A_BLINK); }
			mvwprintw(choice, i+1 , 1, "%s", choices[i].c_str());
		}
		int c = wgetch(choice);
		keypad(choice, 1);
		switch(c){
			case KEY_UP: case 'k': case 'K':
				--hl;
				if (hl > n_choices - 1) hl = 0;
				break;
			case KEY_DOWN: case 'j': case 'J':
				++hl;
				if (hl > n_choices - 1) hl = n_choices - 1;
				break;
			default:
				break;
		}
		if (c == '\n') {wclear(choice); wrefresh(choice); delwin(choice); if (hl == 0) ret = true; break; }
	}
	refresh();
	return ret;
}

void Pomodoro::set_data_from_file(char const* const namefile){
	std::ifstream f(namefile);
	int x, y;
	if (f){
		f>>x;
		f>>y;
	}	
}



void Pomodoro::input_data_to_file(char const* const namefile){
	std::ofstream f(namefile);
	if (f){
		f << this->totalWorkTime.count() << std::endl;
		f << this->sessionsCompleted << std::endl;
	}

}

// πολυ αστειο ονομα επειδη δεν μπορω να βαλω this ως ονομα, αλλα αναφαιρομαι στο παραθυρο για στατιστικα
void Pomodoro::getStatistics(WINDOW* ekeino){
	std::ifstream f("data.txt");
	int x, y;
		if (f){
		f >> x;	//to be deprecated sunarthsh
		f >> y;
		mvwprintw(ekeino, 1,1, "totalWorkTime: %d" ,x);
		mvwprintw(ekeino, 2,1, "sessionsCompleted: %d", y);
		wrefresh(ekeino);
	}
	// read stats from file and print them	
}

/* intended for test cases */


//getters
std::chrono::minutes Pomodoro::GET_breakDuration(void) const {
	return this->breakDuration;
}
std::chrono::minutes Pomodoro::GET_workDuration(void) const {
	return this->workDuration;
}
