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
#define ticker1 9    // opto interruptor on chainring
#define ticker2 10    // opto interruptor on cassette
#define wspeed 11	  // hall effect sensor on R wheel.  
#define LED1 3
#define LED2 4
#define LED3 5
#define LED4 6
#define LED5 7

volatile int tick1 = 0;  //cadence sensor
volatile int tick2 = 0;  // gear calculations
volatile int wcount = 0;  //wheel speed.


int refreshrate = 250;  //   how often it refreshes and recalculates the RPM
long timeperiod;  //  timeperiod is set to millis and overflows after 32 seconds, set as long will run for 20+ days without issues
unsigned long tickold;
int tickdiff;
unsigned int rpm;
long int conversiontick1 = 4000;   // conversionrate for tick 1			needs to be set
long int conversiontick2 = 4000;   // conversionrate for tick 2
long int conversionwcount = 4000;   // conversionrate for wcount
int state;



// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	pinMode(ticker1, INPUT_PULLUP);
	pinMode(LED1, OUTPUT);
	pinMode(LED5, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED3, OUTPUT);
	attachPinChangeInterrupt(ticker1, ticking1, FALLING);
	attachPinChangeInterrupt(ticker2, ticking2, FALLING);
	attachPinChangeInterrupt(wspeed, Wrev, FALLING);				// might need to be a rising signal.  TBD
	timeperiod = millis();
	tickold = tick1;
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (millis() > timeperiod + refreshrate) {


		cadencefunction();
		gearratiofunction();
		gearcalcs();
		printing();
		timeperiod = millis();
	}

}


void cadencefunction()   // cadence calculations and LEDS
{
 {
		tickdiff = tick1 - tickold;
		tickold = tick1;
		rpm = conversiontick1 * tickdiff / refreshrate;  // refreshrate is coming out to be 2001millis which shouldnt really effect the calculation much

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

		timeperiod = millis();

	}
}

void printing()
{

		Serial.println(timeperiod);

		Serial.print(" ticker count is: ");
		Serial.println(tick1);
		Serial.println(tickdiff);

		tickold = tick1;
		Serial.print("RPM is ");
		Serial.println(rpm);
		Serial.println(" ");

}


void ticking1()
{
	tick1++ ;
	digitalWrite(LED5, HIGH);
}
 
void ticking2()
{
	tick2++;
	

}
void Wrev()
{
	wcount ++;
}

void gearratiofunction()
{

}

void gearcalcs()
{

}