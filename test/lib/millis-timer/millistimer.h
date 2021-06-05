#include <Arduino.h>
#include <TimerOne.h>
#include <timer.h>

#ifndef millistimer_h
#define millistimer_h

class MillisTimer : public Timer {
public:

	MillisTimer();
	~MillisTimer();
	
	//overridden methods, time in seconds
	void start(unsigned int time_interval); //starts the count, time_interval in seconds or minutes
	void start(unsigned int time_interval, void (*cb)());
	void restart(); //retakes a stopped count from where it stopped
	void stop();
	void reset();
	unsigned int getTimeLeft(); //same unit as time_interval
	unsigned int getInterval();

private:
	unsigned int time_ref;
	unsigned int time_when_stopped;
	unsigned int time_interval;
	boolean counting;
	boolean stopped;
	

	unsigned int seconds();
};

#endif
