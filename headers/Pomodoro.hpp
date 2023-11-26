#ifndef _POMODORO_HPP
#define _POMODORO_HPP
#include <chrono>
#include <ncurses/curses.h>
class Pomodoro{
	friend void pomodoro_helper_countdown(bool&, size_t& , WINDOW& , bool flag); // flag decides if break or work
	friend void pomodoro_helper_pause_resume(bool&, size_t& );
	friend class MenuBar;
	public:
		Pomodoro();
		Pomodoro(int, int);				/* manages internally the numbers converting them to std::chrono::mins */ 
		void startSession(WINDOW* );  /* starts user's session with given work and break duration */
		void endSession(void);	  /* ends session with the possibility to continue */
		void getStatistics(WINDOW* ); /* prints statistics to given WINDOW* */
		/* intended for test cases */
		std::chrono::minutes GET_breakDuration(void) const;
		std::chrono::minutes GET_workDuration(void) const;
	private:
		std::chrono::minutes workDuration;
		std::chrono::minutes breakDuration;
		size_t sessionsCompleted;
		std::chrono::minutes totalWorkTime;
};
#endif
