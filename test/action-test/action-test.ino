#include <controller.h>
#include <fakeactuator.h>
#include <millistimer.h>
#include <fakesensor.h>

#define TIME 30
#define TLRNC 0.1
#define REF 1.0

FakeSensor *sn = new FakeSensor();
FakeActuator *act= new FakeActuator();
FakeSensor *sn2 = new FakeSensor();
FakeActuator *act2 = new FakeActuator();

Timer *timer = new MillisTimer();

Controller *ctrl = new Controller(timer, new Process(sn, act, REF), TLRNC, TIME);

void printState();
void printProc(Process proc);
void onTimeOver();

boolean isOver = false;

void setup() {
	Serial.begin(9600);
	ctrl->addProc(new Process(sn2, act2, REF));

	Serial.print(F("Numero de processos: "));
	Serial.println(ctrl->getProcsNum());

	for(int i = 0; i < ctrl->getProcsNum(); i++) printProc(ctrl->getProc(i));

  Serial.println(F("Press any key to start the test"));
  while(!Serial.available());

  while(Serial.available()) Serial.read();

  Serial.println(F("Start of action test: "));
  delay(1000);
	Serial.println(ctrl->start());
	Serial.println();
}

void loop() {
  printState();
  delay(900);
  
	if(isOver || ctrl->getState()==REST) return;
  
  ctrl->run();

	if(Serial.available()) {
		String cmd = "";
		float arg = 0;
		cmd += Serial.readStringUntil(' ');

		switch(Serial.readString()[0]) {
			case 'l':
				arg = 0;
				break;
			case 'm':
				arg=1.0;
				break;
			case 'h':
				arg=2.0;
				break;
		}

		Serial.print(cmd);
		Serial.println(" ");
		Serial.print(arg);


		switch(cmd[0]) {
			case 's':
				ctrl->stop();
				break;
			case 'p': //from play
				ctrl->restart();
				break;
			case 'r':
				ctrl->reset();
        isOver = true;
				break;
			case '1':
				sn->setReading(arg);
				break;
			case '2':
				sn2->setReading(arg);
				break;
		}
	}

 if(isOver || ctrl->getState()==REST) Serial.println(F("Test over"));
}



void printState() {
	Serial.print(F("State of the controller: "));
	Serial.println(ctrl->getState());
	Serial.print(F("Time left: "));
	Serial.println(ctrl->getTimeLeft());
	Serial.print(F("Sensor 1: "));
	Serial.println(sn->read());
	Serial.print(F("Actuator 1: "));
	Serial.println(act->getOutput());
	Serial.print(F("Sensor 2: "));
	Serial.println(sn2->read());
	Serial.print(F("Actuator 2: "));
	Serial.println(act2->getOutput());
  Serial.println(F("\n"));
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


void onTimeOver() {
	Serial.println("TIME OVER!");
	isOver = true;
  return;
}
