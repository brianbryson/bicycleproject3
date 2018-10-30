/*
 Name:		bicycleproject3.ino
 Created:	10/27/2018 8:26:58 AM
 Author:	brian
 Version 3: 
*/

#include <PinChangeInt.h>
#define ticker 9
#define LED1 3
#define LED2 4
#define LED3 5
#define LED4 6
#define LED5 7

volatile int tick = 0;


int refreshrate = 2000;
long timeperiod ;
int tickold;
int tickdiff;
int rpm = 0;
long int conversionrate = 60000;    //  converts ticks per refreshrate of 5000 ms  to RPM




// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	pinMode(ticker, INPUT_PULLUP);
	pinMode(LED1, OUTPUT);
	pinMode(LED5, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED3, OUTPUT);
	attachPinChangeInterrupt(ticker, ticking, RISING);

	timeperiod = millis();
	tickold = tick;
}

// the loop function runs over and over again until power down or reset
void loop() {
	
	

	if (millis() > timeperiod + refreshrate) {
		tickdiff = tick - tickold;

		digitalWrite(LED1, LOW);
		digitalWrite(LED2, LOW);
		digitalWrite(LED3, LOW);
		digitalWrite(LED4, LOW);
		digitalWrite(LED5, LOW);

		switch (tickdiff) {
		case 0 ... 2:
			digitalWrite(LED1, HIGH);
			break;
		case 3 ... 4:
			digitalWrite(LED2, HIGH);
			break;
		case 5 ...9 :
			digitalWrite(LED3, HIGH);
			break;
		case 10 ... 100:
			digitalWrite(LED4, HIGH);
			break;
		//case 6 ... 100:
		//	digitalWrite(LED5, HIGH);
		//	break;
		}



		Serial.println(timeperiod);
		timeperiod = millis();
		Serial.print(" ticker count is: ");
		Serial.println(tick);
		Serial.println(tickdiff);

		tickold = tick;
		rpm = 1000*tickdiff / refreshrate;
		Serial.print("RPM is ");
		Serial.println(rpm);
		Serial.println(" ");
	}
}



void ticking()
{
	tick = tick +1 ;
	digitalWrite(LED5, HIGH);
}