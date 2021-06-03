#include "controller.h"

Controller::Controller() {
	this->state = REST;
	ProcNode *node = new ProcNode(NULL);
	this->procs = *node;
	this->tolerance = 0;
	this->timer = NULL;
	this->duration = 0;
}

Controller::Controller(Timer *timer) {
	this->state = REST;
	ProcNode *node = new ProcNode(NULL);
	this->procs = *node;
	this->tolerance = 0;
	this->timer = timer;
	this->duration = 0;
}

Controller::Controller(Timer *timer, Process *proc, float tolerance, unsigned int duration) {
	this->state = REST;
	ProcNode *node = new ProcNode(proc);
	this->procs = *node;
	this->tolerance = tolerance;
	this->timer = timer;
	this->duration = duration;
}

Controller::~Controller() {
	
}

//"control of the controller" (aka start, stop, etc)
boolean Controller::start() {
	if(status!=REST)
		return false;
	if(procs.proc==NULL || timer==NULL)
		return false;

	status=WORKING;
	timer->reset();	
	return run();
}

boolean Controller::restart() {
	if(status!=STOPPED)
		return false;

	timer->restart();
	status=WORKING;
	return run();
}

boolean Controller::stop(){
	if(status!=WORKING)
		return false;
	
	status=STOPPED;
	timer->stop();
	deactivateAll();
	return true;
}

void Controller::reset(){
	status = REST;
	timer->reset();
	deactivateAll();
}

boolean Controller::run(){
	if(status!=WORKING)
		return false;

	if(timer->getInterval()==0) {
		float reading = procs.proc->sensor->read();
		float ref_value = procs.proc->ref_value;
		if(reading > ref_value*(1-tolerance) && reading < ref_value*(1+tolerance))
			timer->start(duration);
	}
	if(timer->getInterval()>0 && timer->getTimeLeft()==0) {
		reset();
	} else {
		ProcNode *node = &node;
		while(node!=null) {
			node->proc->actuator->act(node->proc->sensor->read());
			node=node->next;
		}
	}
}
	
//Manipulation of process (adding, changing, removing or getting them)
	
void Controller::addProc(Process *proc){
	if(proc->sensor == NULL || proc->actuator == NULL)
		return;
	if(state!=REST)
		return;

	boolean alreadyAdded = false;
	ProcNode *node = &procs;
	while(node->next!=NULL) {
		if(node->proc->sensor == proc->sensor && node->proc->actuator == proc->actuator) {
			alreadyAdded = true;
			break;
		}
		node = node->next;
	}

	if(alreadyAdded) {
		node->proc->ref_value = proc->ref_value;
	} else {
		node->next = new ProcNode(proc);
	}
}

void Controller::setProc(unsigned int index, Process *proc){
	if(index >= getProcsNum())
		return;
	if(state!=REST)
		return;

	ProcNode *node = &procs;
	while(index>0) {
		node = node->next;
		index--;
	}
	Process *aux;
	aux=node->proc;
	node->proc = proc;
	delete aux;
}

void Controller::addProc(Sensor *sensor, Actuator *actuator, float ref_value){
	addProc(new Process(sensor, actuator, ref_value));
}

void Controller::setProc(unsigned i nt index, Sensor *sensor, Actuator *actuator, float ref_value){
	setProc(index, new Process(sensor, actuator, ref_value));
}

void Controller::rmvProc(unsigned int index){
	if(index >= getProcsNum())
		return;
	if(procs.proc==NULL)
		return;
	
	ProcNode *toDelete;
	ProcNode *node = &procs;
	if(index==0) {
		toDelete = node;
		procs = *(toDelete->next);
	} else {
		while(index >= 1) {
			index--;
			node = node->next;
		}
		toDelete = node->next;
		node->next = toDelete->next;
	}
	delete toDelete;
}

Process Controller::getProc(unsigned int index){
	if(procs.proc==NULL) {
		Process *p = new Process(NULL, NULL, 0);
		return *p;
	}
	if(index >= getProcsNum())
		index=getProcsNum()-1;
	
	ProcNode *node = &procs;
	while(index > 0) node=node->next;
	
	Process *p = new Process(node->proc->sensor, node->proc->actuator, node->proc->ref_value)
	return *p;
}

void Controller::rmvAllProcs(){
	unsigned int procs_num = getProcsNum();
	while(procs_num>0) {
		rmvProc(0);
		procs_num--;
	}
}

unsigned int Controller::getProcsNum(){
	if(procs.proc==NULL)
		return 0;
	
	unsigned int procs_num = 0;
	ProcNode *node = &procs;
	while(node!=NULL) {
		procsNum++;
		node = node->next;
	}

	return procs_num;
}


//Other methods
unsigned int Controller::getTimeLeft(){
	return timer->getTimeLeft();
}

unsigned int Controller::getDuration(){
	return duration;
}

unsigned short Controller::getState(){
	return state;
}

float Controller::getTolerance() {
	return tolerance;
}

void Controller::setTolerance(float tolerance) {
	this->tolerance = tolerance;
}

//private methods

void Controller::deactivateAll() {
	if(procs.proc==NULL)
		return;
	ProcNode *node = &procs;
	while(node!=NULL) {
		node->proc->actuator->deactivate();
		node=node->next;
	}
}
