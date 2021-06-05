#include "millistimer.h"

MillisTimer::MillisTimer(){
	time_ref = 0;
	time_when_stopped=0;
	time_interval = 0;
	counting = false;
	stopped = false;
}

MillisTimer::~MillisTimer(){
}

	
//overridden methods, time in seconds
void MillisTimer::start(unsigned int time_interval){
	start(time_interval, NULL);
}

void MillisTimer::start(unsigned int time_interval, void (*cb)()) {
	if(counting)
		return;

	time_ref = seconds();
	time_when_stopped = time_ref;
	this->time_interval = time_interval;
	counting = true;
	stopped = false;
	this->cb = cb;

	if(cb != NULL) {
		unsigned long t = 1000*time_interval;
		Timer1.attachInterrupt(cb, t);
	}
}

void MillisTimer::restart(){
	if(!counting || !stopped)
		return;
	time_ref += seconds() - time_when_stopped;
	time_when_stopped = time_ref;
	stopped=false;
	if(cb!=NULL) {
		unsigned long t = 1000*(getTimeLeft());
		Timer1.attachInterrupt(cb, t);
	}
}

void MillisTimer::stop(){
	if(!counting || stopped)
		return;
	time_when_stopped = seconds();
	Timer1.detachInterrupt();
	stopped = true;
}

void MillisTimer::reset(){
	time_interval = 0;
	time_ref = 0;
	time_when_stopped = 0;
	counting = false;
	stopped = false;
	Timer1.detachInterrupt();
	cb = NULL;
}

unsigned int MillisTimer::getTimeLeft(){
	unsigned int time_left = 0;
	if(counting) {
		if(stopped)
			time_left = time_interval-time_when_stopped+time_ref;
		else 
			time_left = (time_interval-seconds()+time_ref);
			
		if(time_left < 0) time_left = 0;
	}

	return time_left;
}

unsigned int MillisTimer::getInterval(){
	return time_interval;
}

//private methods
unsigned int MillisTimer::seconds() {
	unsigned long ms = millis()/1000;
	unsigned int s = (unsigned int) ms;
	return s;
}
