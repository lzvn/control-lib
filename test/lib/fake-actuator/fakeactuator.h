#include <Arduino.h>
#include <actuator.h>

#ifndef fakeactuator_h
#define fakeactuator_h

class FakeActuator : public Actuator {
public:

	FakeActuator();
	~FakeActuator();

	//interface methods
	void act(float input); //acts according to the input
	void activate(); //activate the output regardless of input
	void deactivate(); //deactivate the output
	float getOutput(); //gets the current value of the output
	float getRefValue();
	void setRefValue(float ref_value);

private:
	boolean active;
	float ref_value;
};

#endif
