#include <Arduino.h>
#include <actuator.h>

#ifndef ledactuator_h
#define ledactuator_h

class LedActuator : public Actuator {
public:

	LedActuator(unsigned int pin, float tolerance);
	~LedActuator();

	//interface methods
	void act(float input); //acts according to the input
	void activate(); //activate the output regardless of input
	void deactivate(); //deactivate the output
	float getOutput(); //gets the current value of the output
	float getRefValue();
	void setRefValue(float ref_value);

private:
	unsigned int pin;
	boolean active;
	float ref_value;
};

#endif
