#ifndef CONFIG_HPP
#define CONFIG_HPP

using namespace std;
#include "states.hpp"

class config{
	private:
		string filename;
		ConfigStates configState;
		char currDate[10];
		char scheduleName[50];
		char scheduleDir[50];
		char newDay[10];
		int bell_duration;
	protected:
		int bellCount;
	public:
		bellProps *bellsTime;
		config();
		void setBellCount();
		int getBellCount();
		void createBellsTime();
		bellProps getBellsTime();
		string getScheduleFileName();
		void setScheduleFileName(string filename);
		ConfigStates getState();
		void waitForNewDay();
		int setSchedule();
};

#endif // CONFIG_HPP
