#include "../headers/Pomodoro.hpp"
#include <chrono>
Pomodoro::Pomodoro() : workDuration(std::chrono::minutes(25)), breakDuration(std::chrono::minutes(5)) {}
Pomodoro::Pomodoro(int wd, int bd){
	using namespace std::chrono; /* it's inside a function so it wont ruin everything */
if (wd < 1) this->workDuration = minutes(1);
else if (wd > 1440) this->workDuration = minutes(1440);
else this->workDuration = minutes(wd);

if (bd < 1) this->breakDuration = minutes(1);
else if (bd > 1440) this->breakDuration = minutes(1440);
else this->breakDuration = minutes(bd);

}
void Pomodoro::startSession(void){

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
