#include "controller.h"

Controller::Controller() {
	this->state = REST;
	ProcNode *node = new ProcNode(NULL);
	this->procs = *node;
	this->timer = NULL;
	this->duration = 0;
}

Controller::Controller(Timer *timer) {
	this->state = REST;
	ProcNode *node = new ProcNode(NULL);
	this->procs = *node;
	this->timer = timer;
	this->duration = 0;
}

Controller::Controller(Timer *timer, Process *proc, unsigned int duration) {
	this->state = REST;
	ProcNode *node = new ProcNode(proc);
	this->procs = *node;
	this->timer = timer;
	this->duration = duration;
}

Controller::~Controller() {
	
}
//"control of the controller" (aka start, stop, etc)
boolean Controller::start() {
	
}

boolean Controller::restart() {
	
}

boolean Controller::stop(){
}

void Controller::reset(){
}

boolean Controller::run(){
}
 //updates the state of all processes
	
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
 //delete all auxiliary process too
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
	while(node->next!=NULL) {
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

