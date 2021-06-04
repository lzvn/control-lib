#include <Arduino.h>
#include <sensor.h>

#ifndef detector_h
#define detector_h

class Detector: public Sensor {
public:
	Detector(unsigned int pin);
	~Detector();

	//interface methods
	float read(); //gets the reading of the sensor
	void calibrate(); //calibrate the sensor, whatever it means for a given sensor
private:
	unsigned int pin;
};

#endif
