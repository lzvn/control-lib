#include "fakeactuator.h"

FakeActuator::FakeActuator(){
	this->active = false;
	this->ref_value = 0;
}

FakeActuator::~FakeActuator(){
}

//interface methods
void FakeActuator::act(float input){
	if(input!=ref_value)
		activate();
	else
		deactivate();
}

void FakeActuator::activate(){
	active = true;
}

void FakeActuator::deactivate(){
	active = false;
}

float FakeActuator::getOutput(){
	return (active?1.0:0);
}

float FakeActuator::getRefValue(){
	return ref_value;
}

void FakeActuator::setRefValue(float ref_value){
	this->ref_value = ref_value;
}
