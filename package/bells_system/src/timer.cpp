#include <iostream>
#include <unistd.h>
#include <ctime>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include "config.hpp"
#include "timer.hpp"
#include "GPIOClass.hpp"
#include "states.hpp"

#define bell_block_value 30

using namespace std;

bool tmr_done = false;
int default_ring_duration = 5;
int ring_duration = 5;

void startRing(int, siginfo_t *, void *){
	syslog("bell is ringing now");
	
	GPIOClass * tmpGpio = new GPIOClass;
	tmpGpio->export_gpio();
	tmpGpio->setdir_gpio("out");
	
	timer * tmpTmr = new timer;
	while(1){
		tmpGpio->setval_gpio("1");
		sleep(ring_duration);
		tmpGpio->setval_gpio("0");
		break;
	}
	tmr_done = true;
	delete tmpTmr;
}

timer::timer(){
	timerCount = 0;
	ringTime = 0;
}

TimerErr timer::initTimer(){
	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = startRing;
	
	memset(&sev, 0, sizeof(sev));
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_value.sival_ptr = (void*) this;
	sev.sigev_signo = SIGALRM;
	
	if(sigaction(SIGALRM, &sa, NULL)  == 0 ){
		state = TMR_INITED;
	}
	return ER_OK;
}

TimerErr timer::stopTimer(){
	
	if(timer_delete(timerID) == -1){
		return ER_TIMER_NOT_DELETED;
	}
	syslog("bell was given manually, blocking timer");
	tmr_done = true;
	state = TMR_STOPPED;
	return ER_OK;
}

TimerErr timer::startTimer(config * obj){
		
	if(timer_create(CLOCK_REALTIME, &sev, &timerID) != 0){
		return ER_TIMER_NOT_INITED;
	}
	
	setRingTime(obj);
	if(ringTime <= 0){
		state = TMR_NEG_RINGTIME;
		return ER_TIMER_NOT_CREATED;
	}
	memset(&ts, 0, sizeof(ts));
	this->ts.it_value.tv_sec = ringTime;
	if( timer_settime(this->timerID, 0, &ts, 0) == -1 ){
		return ER_TIMER_NOT_STARTED;
	}
	syslog("timer has been set");
	state = TMR_STARTED;
	return ER_OK;
}

int timer::getRingTime(){ 
	return ringTime; 
}

TimerStates timer::getState(){
	return state;
}

void timer::incTimerCounter(){
	this->timerCount++;
}

int timer::getTimerCount(){
	return timerCount;
}

void timer::setRingTime(config * obj){
	
	if (obj->getBellCount() == 0){
		return;
	}
	
	incTimerCounter();
	
	obj->bellsTime[timerCount-1].bell_duration < 1 ||
	obj->bellsTime[timerCount-1].bell_duration > 15 ?
		ring_duration = default_ring_duration : ring_duration = obj->bellsTime[timerCount-1].bell_duration;
	
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	int hour = obj->bellsTime[timerCount-1].hour;
	int minute = obj->bellsTime[timerCount-1].minute;
	if( timerCount <= obj->getBellCount() ){
		ringTime = (hour*60*60 + minute*60) - (tm->tm_hour *60*60 + tm->tm_min *60 + tm->tm_sec);
	}else{
		syslog("No more bells for this day. Waiting for new day...");
	}
}

void timer::resetTmrProps(){
	timerCount = 0;
	ringTime = 0;
}

TimerStates timer::block_timer(){
	if(ringTime <= bell_block_value){
		stopTimer();
		return TMR_BLOCKED;
	}
	return TMR_NOT_BLOCKED;
}
