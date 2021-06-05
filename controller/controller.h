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
	ProcNode() {
		this->proc = NULL;
		this->next = NULL;
	}
};

class Controller {
public:
	Controller();
	Controller(Timer *timer);
	Controller(Timer *timer, Process *proc, float tolerance, unsigned int duration);
	~Controller();

	//"control of the controller" (aka start, stop, etc)
	boolean start(void (*cb)());
	boolean start();
	boolean restart();
	boolean stop();
	void reset();
	boolean run(); //updates the state of all processes

	/* think if these are necessary or not
	   boolean activateActuatorOf(unsigned int index);
	   boolean deactivateActuatorOf(unsigned int index);
	 */
	
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
	float getTolerance();
	void setTolerance(float tolerance);
	
private:
	/*Linked list of all processes controlled, the first one is considered the 
	  main process and it is the one used to decide when the timer will be started
	*/
	ProcNode procs;
	Timer *timer;
	float tolerance;
	unsigned short state;
	unsigned int duration;
	void (*cb)();

	void deactivateAll();
	void setActuators();
};

#endif
