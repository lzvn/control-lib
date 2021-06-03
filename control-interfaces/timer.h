#include <Arduino.h>

#ifndef timer_h
#define timer_h

class Timer {
public:
	virtual void start(unsigned int time_interval) = 0; //starts the count, time_interval in seconds or minutes
	virtual void restart() = 0; //retakes a stopped count from where it stopped
	virtual void stop() = 0;
	virtual void reset() =  0;
	virtual unsigned int getTimeLeft() = 0; //same unit as time_interval
	virtual unsigned int getInterval() = 0;
};

#endif
