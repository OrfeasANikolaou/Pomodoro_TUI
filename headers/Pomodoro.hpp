#include <chrono>
using namespace std::chrono;
class Pomodoro{
	public:
		Pomodoro();
		Pomodoro(minutes, minutes); 
		void startSession(void);
		void endSession(void);
		void getStatistics(void);
	private:
		minutes workDuration;
		minutes breakDuration;
		size_t sessionsCompleted;
		minutes totalWorkTime;
};
