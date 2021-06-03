#include <Arduino.h>

#include <sensor.h>
#include <timer.h>
#include <actuator.h>

#ifndef controller_h
#define controller_h

enum ControllerStates {REST, WORKING, STOPPED, ERROR};

struct Process {
	Sensor *sensor;
	Actuator *actuator;
	float ref_value;
	Process(Sensor *sensor, Actuator *actuator, float ref_value) {
		this->sensor = sensor;
		this->actuator = actuator;
		this->ref_value = ref_value;
	}
};

typedef struct ProcNode{
	Process *proc;
	ProcNode *next;
	ProcNode(Process *proc) {
		this->proc = proc;
		this->next = NULL;
	}
};

class Controller {
public:
	Controller();
	Controller(Timer *timer);
	Controller(Timer *timer, Process *proc, unsigned int duration);
	~Controller();

	//"control of the controller" (aka start, stop, etc)
	boolean start();
	boolean restart();
	boolean stop();
	void reset();
	boolean run(); //updates the state of all processes
	
	//Manipulation of process (adding, changing, removing or getting them)
	//index starts at 0
	void addProc(Process *proc);
	void setProc(unsigned int index, Process *proc);
	void addProc(Sensor *sensor, Actuator *actuator, float ref_value);
	void setProc(unsigned int index, Sensor *sensor, Actuator *actuator, float ref_value);
	void rmvProc(unsigned int index); //delete all auxiliary process too
	Process getProc(unsigned int index);
	void rmvAllProcs();
	unsigned int getProcsNum();

	//Other methods
	unsigned int getTimeLeft();
	unsigned int getDuration();
	unsigned short getState();
	
private:
	ProcNode procs;
	Timer *timer;
	unsigned short state;
	unsigned int duration;
};

#endif
