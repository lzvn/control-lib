#include <controller.h>
#include <ledactuator.h>
#include <millistimer.h>
#include <detector.h>

#define LED 10
#define BTN 0
#define LED2 11
#define BTN2 1

#define TLRNC 0.1
#define REF 1.0

Sensor *sn = new Detector(BTN);
Actuator *led = new LedActuator(LED, TLRNC);
Sensor *sn2 = new Detector(BTN2);
Actuator *led2 = new LedActuator(LED2, TLRNC);

Timer *timer = new MillisTimer();

Controller *ctrl = new Controller(timer, new Process(sn, led, REF), TLRNC, 120);

void printProc(Process proc);

void setup() {
	Serial.begin(9600);
	Serial.println(F("Testing of manipulation of processes\n"));

 
	Serial.print(F("Number of processes: "));
	Serial.println(ctrl->getProcsNum());
	printProc(ctrl->getProc(0));
  
	ctrl->addProc(sn, led, 2);
	Serial.print(F("Number of processes after failed addition: "));
	Serial.println(ctrl->getProcsNum());
  
	ctrl->addProc(sn2, led2, 5);
	Serial.print(F("Number of processes after successful addition: "));
	Serial.println(ctrl->getProcsNum());

	Serial.println(F("Processes: "));

	for(int i = 0; i < ctrl->getProcsNum(); i++) printProc(ctrl->getProc(i));

	ctrl->rmvProc(0);
	Serial.print(F("Number of processes after removing the first: "));
	Serial.println(ctrl->getProcsNum());
	printProc(ctrl->getProc(0));

	ctrl->addProc(sn, led, 3);
  Serial.print(F("Number of processes after adding the first to the end again: "));
  Serial.println(ctrl->getProcsNum());
	ctrl->rmvProc(1);
	Serial.print(F("Number of processes after adding the first to the end and then deleting it again: "));
	Serial.println(ctrl->getProcsNum());

	Serial.println(F("End of test"));	
}

void loop() {
	return;
}

void printProc(Process proc) {
	Serial.print(F("{ *sensor: "));
	Serial.print((int) proc.sensor);
	Serial.print(F(", *actuator: "));
	Serial.print((int) proc.actuator);
	Serial.print(F(", ref_value: "));
	Serial.print(proc.ref_value);
	Serial.println(F(" }"));
}
