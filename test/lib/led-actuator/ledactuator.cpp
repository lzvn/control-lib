#include "ledactuator.h"

LedActuator::LedActuator(unsigned int pin, float tolerance){
	this->pin = pin;
	this->active = false;
	
	pinMode(pin, OUTPUT);
}

LedActuator::~LedActuator(){
}

//interface methods
void LedActuator::act(float input){
	if(input!=ref_value)
		activate();
	else
		deactivate();
}

void LedActuator::activate(){
	digitalWrite(pin, HIGH);
	active = true;
}

void LedActuator::deactivate(){
	digitalWrite(pin, LOW);
	active = false;
}

float LedActuator::getOutput(){
	return (active?1.0:0);
}

float LedActuator::getRefValue(){
	return ref_value;
}

void LedActuator::setRefValue(float ref_value){
	this->ref_value = ref_value;
}
