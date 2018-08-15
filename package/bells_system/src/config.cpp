#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include "config.hpp"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

config::config(){
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	bellCount = 0;
	memset(newDay, 0, sizeof(newDay));
	
	sprintf(scheduleDir, "%s", "/root/.bellsSystem/schedules/");
	sprintf(currDate, "%d_%d", tm->tm_mday, tm->tm_mon+1);
	setSchedule();
	setBellCount();
	
	for(int i = 0; i < bellCount; i++){
		bellsTime = new bellProps[bellCount];
	}
	configState = CONF_INITED;
}

void config::setBellCount(){
	string tmp;
	ifstream fin(filename.c_str());
	cin.clear();
	while(getline(fin, tmp)){
		bellCount++;
	}
}

void config::createBellsTime(){
	string buff;
	char buff2[6] = "";
	ifstream fin(this->filename.c_str());
	int i = 0;
	for(i=0; i < bellCount; i++){
		fin >> buff;
		strcpy(buff2, buff.c_str());
		sscanf(buff2, "%d:%d;%d;", &bellsTime[i].hour, &bellsTime[i].minute, &bellsTime[i].bell_duration);
	}
	configState = SCHEDULE_CREATED;
}

bellProps config::getBellsTime(){
	return *bellsTime;
}

string config::getScheduleFileName(){
	return filename;
}

int config::getBellCount(){
	return bellCount;
}		

void config::setScheduleFileName(string filename){
	this->filename = filename;
}

void config::waitForNewDay(){
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	sprintf(newDay, "%d_%d", tm->tm_mday, tm->tm_mon+1);
	if(strcmp(newDay, currDate) != 0){
		syslog("new day has come");
		sprintf(newDay, "%d_%d", tm->tm_mday, tm->tm_mon+1);
		configState = NEW_DAY;
		setSchedule();
	}
}

ConfigStates config::getState(){
	return configState;
}

int config::setSchedule(){
	
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	bool daySched = false, weekSched = false, newDayCheck = false;
	
	DIR * dirp;
	struct dirent *dir;
	dirp = opendir(scheduleDir);
	
	while((dir=readdir(dirp)) != NULL){
		if (configState == NEW_DAY && newDayCheck == false){
			sprintf(currDate, "%s", newDay);
			newDayCheck = true;
		}
		if (strcmp(dir->d_name, currDate) == 0){
			syslog("schedule for this date was found. setting...");
			sprintf(scheduleName, "%s%s", scheduleDir, currDate);
			setScheduleFileName(scheduleName);
			daySched = true;
		}
	}
		
	if(!daySched){
		char dayName[10];
		strftime(dayName, 10, "%A", tm);
		string tmp = scheduleDir + string("default/") + dayName;
		if ( access(tmp.c_str(), 0) != -1){
			syslog("setting schedule for this weekday");
			setScheduleFileName(tmp);
			weekSched = true;
		}
	}
	
	if(!daySched && !weekSched){
		syslog("nothing special schedule for this day. setting to default...");
		sprintf(scheduleName, "%s", "/root/.bellsSystem/schedules/default/default");
		setScheduleFileName (scheduleName);
	}
	
	closedir(dirp);
	return 0;
}
