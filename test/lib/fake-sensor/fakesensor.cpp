#include "fakesensor.h"

FakeSensor::FakeSensor(){
	this->reading = 0;
}

FakeSensor::~FakeSensor(){
}

void FakeSensor::setReading(float reading) {
	this->reading = reading;
}

//interface methods
float FakeSensor::read(){
	return reading;
}

void FakeSensor::calibrate(){
}
