#include <Arduino.h>

#ifndef actuator_h
#define actuator_h

class Actuator {
public:
	virtual void act(float input) = 0; //acts according to the input
	virtual void activate() = 0; //activate the output regardless of input
	virtual void deactivate() = 0; //deactivate the output
	virtual float getOutput() = 0; //gets the current value of the output
	virtual float getRefValue() = 0;
	virtual void setRefValue(float ref_value) = 0;
};

#endif
