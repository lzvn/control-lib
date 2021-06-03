#include <Arduino.h>

#ifndef sensor_h
#define sensor_h

class Sensor {
public:
	virtual float read() = 0; //gets the reading of the sensor
	virtual void calibrate() = 0; //calibrate the sensor, whatever it means for a given sensor
};

#endif
