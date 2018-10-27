/*
 Name:		bicycleproject3.ino
 Created:	10/27/2018 8:26:58 AM
 Author:	brian
 Version 3: 
*/

#include <PinChangeInt.h>
#define ticker 2
#define LED1 3
#define LED5 4
#define LED2 6
#define LED4 8
#define LED3 10

volatile int tick = 0;


int refreshrate = 5000;
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
	attachPinChangeInterrupt(ticker, ticking, FALLING);

	timeperiod = millis();
	tickold = tick;
}

// the loop function runs over and over again until power down or reset
void loop() {

	
	if (millis() > timeperiod + refreshrate) {
		tickdiff = tick - tickold;
		if (tickdiff > 3) {
			digitalWrite(LED1, HIGH);
			tickold = tick;
		}
		else
		{
			digitalWrite(LED1, LOW);
			tickold = tick;
		}




		Serial.println(timeperiod);
		timeperiod = millis();
		Serial.print(" ticker count is: ");
		Serial.println(tick);
		Serial.println(tickdiff);
		Serial.println(" ");
	}
}



void ticking()
{
	tick = tick +1 ;
}