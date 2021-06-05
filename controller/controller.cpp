#include "controller.h"

Controller::Controller() {
	this->state = REST;
	ProcNode *node = new ProcNode();
	this->procs = *node;
	this->tolerance = 0;
	this->timer = NULL;
	this->duration = 0;
}

Controller::Controller(Timer *timer) {
	this->state = REST;
	ProcNode *node = new ProcNode();
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
boolean Controller::start(void (*cb)()) {
	if(state!=REST)
		return false;
	if(procs.proc==NULL || timer==NULL)
		return false;

	state=WORKING;
	timer->reset();
	setActuators();
	this->cb = cb;
	return run();	
}
boolean Controller::start() {
	return start(NULL);
}

boolean Controller::restart() {
	if(state!=STOPPED)
		return false;

	timer->restart();
	state=WORKING;
	return run();
}

boolean Controller::stop(){
	if(state!=WORKING)
		return false;
	
	state=STOPPED;
	timer->stop();
	deactivateAll();
	return true;
}

void Controller::reset(){
	state = REST;
	timer->reset();
	deactivateAll();
}

boolean Controller::run(){
	if(state!=WORKING)
		return false;

	if(timer->getInterval()==0) {
		float reading = procs.proc->sensor->read();
		float ref_value = procs.proc->ref_value;
		if(reading > ref_value*(1-tolerance) && reading < ref_value*(1+tolerance))
			timer->start(duration, cb);
	}
	if(timer->getInterval()>0 && timer->getTimeLeft()==0) {
		reset();
	} else {
		ProcNode *node = &procs;
		while(node!=NULL) {
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
	ProcNode *previous = &procs;
	while(node!=NULL) {
		if(node->proc->sensor == proc->sensor && node->proc->actuator == proc->actuator) {
			alreadyAdded = true;
			break;
		}
		previous = node;
		node = node->next;
	}

	if(alreadyAdded) {
		node->proc->ref_value = proc->ref_value;
	} else {
		previous->next = new ProcNode(proc);
		previous->next->next = NULL; //find out why I ned to put this here
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

void Controller::setProc(unsigned int index, Sensor *sensor, Actuator *actuator, float ref_value){
	setProc(index, new Process(sensor, actuator, ref_value));
}

void Controller::rmvProc(unsigned int index){
	if(index >= getProcsNum()) {
		return;
	}
	if(procs.proc==NULL) {
		return;
	}

	boolean lastProc = (index == getProcsNum()-1);
	
	ProcNode *toDelete;
	ProcNode *node = &procs;
	if(index==0) {
		toDelete = node;
		node = toDelete->next;
	} else {
		while(index > 1) {
			index--;
			node = node->next;
		}
		toDelete = node->next;
		node->next = toDelete->next;
		
		if(lastProc) {
			node->next = NULL;
		}
	}

	delete toDelete;

	if(index==0) {
		procs.proc = node->proc;
		procs.next = node->next;
	}
}

Process Controller::getProc(unsigned int index){
	if(procs.proc==NULL) {
		Process *p = new Process(NULL, NULL, 0);
		return *p;
	}
	if(index >= getProcsNum())
		index=getProcsNum()-1;
	
	ProcNode *node = &procs;
	while(index > 0) {
		node=node->next;
		index--;
	}
	
	Process *p = new Process(node->proc->sensor, node->proc->actuator, node->proc->ref_value);
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
		procs_num++;
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

void Controller::setActuators() {
	ProcNode *node = &procs;
	while(node!=NULL) {
		node->proc->actuator->setRefValue(node->proc->ref_value);
		node = node->next;
	}
}
