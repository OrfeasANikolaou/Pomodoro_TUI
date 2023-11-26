#define CATCH_CONFIG_MAIN
#include "../headers/third_party/Catch2/catch.hpp"
#include "../headers/Pomodoro.hpp"
#include <chrono>

//different simple unit tests to show how the constructo works

TEST_CASE("test_case_pomodoro_constructor_1", "[pomodoro_constructor]"){
	using namespace std::chrono;
	
	Pomodoro obj(0, 0);
	REQUIRE(obj.GET_breakDuration() == minutes(5)); 
	REQUIRE(obj.GET_workDuration() == minutes(25));
	
}
TEST_CASE("test_case_pomodoro_constructor_2", "[pomodoro_constructor]"){
	using namespace std::chrono;
	
	
	Pomodoro obj2(4, 3);
	REQUIRE(obj2.GET_workDuration() == minutes(4));
	REQUIRE(obj2.GET_breakDuration() == minutes(3));
	REQUIRE(1 == true);
}
TEST_CASE("test_case_pomodoro_constructor_3", "[pomodoro_constructor]"){
	using namespace std::chrono;
	
	Pomodoro obj3(3, -2);
	REQUIRE(obj3.GET_breakDuration() == minutes(5));
	REQUIRE(obj3.GET_workDuration() == minutes(3));
	
}
TEST_CASE("test_case_pomodoro_constructor_4", "[pomodoro_constructor]"){
	using namespace std::chrono;
	
	Pomodoro obj4(234576, 21344);
	REQUIRE(obj4.GET_workDuration() == minutes(1440));
	REQUIRE(obj4.GET_breakDuration() == minutes(1440));
}
TEST_CASE("test_case_pomodoro_constructor_5", "[pomodoro_constructor]""[default_pomodoro_constructor]"){
	using namespace std::chrono;
	Pomodoro obj5;
	REQUIRE(obj5.GET_breakDuration() == minutes(5));
	REQUIRE(obj5.GET_workDuration() == minutes(25));
}


