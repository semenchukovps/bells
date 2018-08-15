#include <iostream>
#include "timer.hpp"
#include "config.hpp"
#include "GPIOClass.hpp"
#include <string>
#include <unistd.h>
#include <cerrno>

/* ввести:
 + обработчик сингалов SIG_TERM и т.д.
 * ввести аргмуенты командной строки, для того что б была возможно выбора запуска в режиме демона или обычном
 +- уровен отладочных сообщений
 * обесечить возможность запуска в режиме демона, с генерацией pid-файла и т.д.
 * избавиться от pause перейти в режим активного идания, то есть засыпать максимум на одну секунду
 * реализовать интерфейс для взаимодействия с внешим миром, через именвоанный канал.
   то есть демон должен уметь принмать команды такие как перечитать (настройки для начала) 
*/

using namespace std;

bool bellContinue = true;

void quitHandle(int signum){
	bellContinue = false;
}

int main(int argc, char **argv){
	
	syslog("started working");		
	config * conf = new config();
	timer * tmr = new timer();
	GPIOClass * gpio4 = new GPIOClass("4");
  GPIOClass * gpio17 = new GPIOClass("17");
 	
	conf->createBellsTime();	
	tmr->initTimer();
	
	gpio4->export_gpio();
  gpio17->export_gpio();
  
  gpio4->setdir_gpio("out");
  gpio17->setdir_gpio("in");
		
	signal(SIGINT, quitHandle);
	signal(SIGTERM, quitHandle);	
	
	while (bellContinue){
		while(tmr->getTimerCount() >= conf->getBellCount()){
			conf->waitForNewDay();
			gpio17->scan_gpio_input();
			gpio17->get_gpio_state() == BUTTON_PRESSED ? gpio4->setval_gpio("1") : gpio4->setval_gpio("0");
			if(conf->getState() == NEW_DAY)	tmr->resetTmrProps();
		}
		 
		tmr->startTimer(conf);
		if(tmr->getState() != TMR_STARTED) continue;		
		
		while(!tmr_done){
			gpio17->scan_gpio_input();
			if(gpio17->get_gpio_state() == BUTTON_PRESSED){
				gpio4->setval_gpio("1");
				tmr->block_timer();
			}else{
				gpio4->setval_gpio("0");
			}
		}
		tmr_done = false;
	}
	
	gpio4->unexport_gpio();
	gpio17->unexport_gpio();
	delete conf;
	delete tmr;
	delete gpio4;
	delete gpio17;

	return 0;
}
