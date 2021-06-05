#include <Arduino.h>
#include <sensor.h>

#ifndef fakesensor_h
#define fakesensor_h

class FakeSensor : public Sensor {
public:
	FakeSensor();
	~FakeSensor();

	void setReading(float reading);

	//interface methods
	float read(); //gets the reading of the sensor
	void calibrate(); //calibrate the sensor, whatever it means for a given sensor
private:
	float reading;
};

#endif
