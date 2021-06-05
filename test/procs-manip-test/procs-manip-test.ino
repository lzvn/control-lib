#include <controller.h>
#include <fakeactuator.h>
#include <millistimer.h>
#include <fakesensor.h>

#define TLRNC 0.1
#define REF 1.0

FakeSensor *sn = new FakeSensor();
FakeActuator *led = new FakeActuator();
FakeSensor *sn2 = new FakeSensor();
FakeActuator *led2 = new FakeActuator();

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
