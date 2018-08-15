#include <iostream>
#include <cstdlib>
#include "states.hpp"
#include <stdio.h>
#include <string.h>

void syslog(string s){
	FILE *logfile = fopen("/root/.bellsSystem/bell.log", "a+");
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	fflush(stdout);
	fprintf(logfile, "[%d.%d.%d] %d:%d:%d - %s \n" , tm->tm_mday, tm->tm_mon +1 , 
					tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec, (char*)s.c_str());
	fclose(logfile);
}


