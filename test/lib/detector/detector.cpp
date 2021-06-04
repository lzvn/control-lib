#include "detector.h"

Detector::Detector(unsigned int pin){
	this->pin = pin;
	pinMode(pin, INPUT);
}

Detector::~Detector(){
}


//interface methods
float Detector::read(){
	return (digitalRead(pin)?1.0:0);
}

void Detector::calibrate(){
}
