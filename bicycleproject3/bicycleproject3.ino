/*
 Name:		bicycleproject3.ino
 Created:	10/27/2018 8:26:58 AM
 Author:	brian
 Version 3: 
 RPM is currently being calculated as 1 trip of the sensor as a revolution
 functions working.  need to calculate conversion rate and change refRate to ~500 or possibly more like 250

 each iteration of the RPM calculation is about 1ms			not sure what to do about that or if i need to do anything. probably not yet.
*/

#include <PinChangeInt.h>
#define ticker 9
#define LED1 3
#define LED2 4
#define LED3 5
#define LED4 6
#define LED5 7

volatile int tick = 0;


int refreshrate = 250;  //   how often it refreshes and recalculates the RPM
long timeperiod;
unsigned long tickold;
int tickdiff;
unsigned int rpm;
long int conversionrate = 4000;    //  converts ticks per refreshrate of 5000 ms  to RPM
int x;



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

cadencefunction();
printing();  

}


void cadencefunction()
{
	if (millis() > timeperiod + refreshrate) {
		tickdiff = tick - tickold;
		tickold = tick;
		rpm = conversionrate * tickdiff / refreshrate;  // refreshrate is coming out to be 2001millis which shouldnt really effect the calculation much

		digitalWrite(LED1, LOW);
		digitalWrite(LED2, LOW);
		digitalWrite(LED3, LOW);
		digitalWrite(LED4, LOW);
		digitalWrite(LED5, LOW);

		switch (rpm) {
		case 0 ... 2:
			digitalWrite(LED1, HIGH);
			break;
		case 3 ... 4:
			digitalWrite(LED2, HIGH);
			break;
		case 5 ...9:
			digitalWrite(LED3, HIGH);
			break;
		case 10 ... 100:
			digitalWrite(LED4, HIGH);
			break;
			//case 6 ... 100:
			//	digitalWrite(LED5, HIGH);
			//	break;
		}

		x++;
		timeperiod = millis();

	}
}

void printing()
{
	if (x >= 4) {
		Serial.println(timeperiod);

		Serial.print(" ticker count is: ");
		Serial.println(tick);
		Serial.println(tickdiff);

		tickold = tick;
		Serial.print("RPM is ");
		Serial.println(rpm);
		Serial.println(" ");
		x = 0;
	}
}


void ticking()
{
	tick = tick +1 ;
	digitalWrite(LED5, HIGH);
}