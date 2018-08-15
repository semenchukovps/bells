#ifndef TIMER_HPP
#define TIMER_HPP

#include "config.hpp"
#include <csignal>
#include <ctime>

extern int default_ring_duration;
extern int ring_duration;
extern bool tmr_done;
extern void func(int, siginfo_t *, void *);

class timer{
	private:
		TimerStates state;
		time_t ringTime;
		timer_t timerID;
		bool interrupted;
		struct sigaction sa;
		struct sigevent sev;
		struct itimerspec ts;
		int timerCount;
	protected:	
		bellProps * bellsTime;
	public:
		timer();
		timer(clockid_t ringTime, timer_t ringTimer);
		TimerErr initTimer(); 
		TimerErr stopTimer();
		TimerErr startTimer(config * obj);
		int getRingTime();
		TimerStates getState();
		void incTimerCounter();
		int getTimerCount();
		void stopRing();
		void setRingTime(config * obj);
		void resetTmrProps();
		TimerStates block_timer();
		int get_bell_duration();
		void init_static_props();
};



#endif /* TIMER_HPP */
