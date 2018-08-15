#ifndef STATES_HPP
#define STATES_HPP

#include <fstream>

using namespace std;

typedef enum{
	TMR_CREATED,
	TMR_INITED,
	TMR_STARTED,
	TMR_STOPPED,
	TMR_NEG_RINGTIME,
	TMR_DONE,
	TMR_BLOCKED,
	TMR_NOT_BLOCKED
}TimerStates;

typedef enum{
	CONF_INITED,
	SCHEDULE_CREATED,
	NEW_DAY,
	EMPTY_SCHEDULE
}ConfigStates;

typedef enum{
	ER_OK,
	ER_TIMER_NOT_INITED,
	ER_TIMER_NOT_CREATED,
	TIMER_START_FILE,
	ER_TIMER_NOT_DELETED,
	ER_TIMER_NOT_STARTED,
}TimerErr;

typedef struct{
	int hour; 
	int minute;
	int bell_duration;
}bellProps;

typedef enum{
	BUTTON_PRESSED,
	BUTTON_NOT_PRESSED	
}GPIOStates;

void syslog(string s);


#endif // STATES_HPP
