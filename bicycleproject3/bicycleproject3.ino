/*
 Name:		bicycleproject3.ino
 Created:	10/27/2018 8:26:58 AM
 Author:	brian
 Version 3: 


 redid for school projet. self shifting.  need to focus on that.
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
long timeperiod; // being used to compare to millis
int refreshrate = 2000;  //   how often it refreshes and recalculates the RPM



//		ints for cadence function
unsigned long tickold1;
int tickdiff1;
unsigned int rpm1;
long int conversiontick1 = 500;   // conversionrate for tick 1			needs to be set


//			ints for gear shifter
int currentgear;																// needs to be set so it always stays with the program
int gearrange;																	// +- range before shifts occur
int goalrpm;																		// ideal RPM


//		ints for gearratio functions
long int conversiontick2 = 4000;   // conversionrate for tick 2
int tickdiff2;
unsigned int rpm2;
float(ratio);
unsigned long tickold2;




//			ints for speedcalc
long int conversionwcount = 4000;   // conversionrate for wcount




// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	pinMode(ticker1, INPUT);
	pinMode(ticker2, INPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED5, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED3, OUTPUT);
	attachPinChangeInterrupt(ticker1, ticking1, FALLING);
	attachPinChangeInterrupt(ticker2, ticking2, FALLING);
	attachPinChangeInterrupt(wspeed, Wrev, FALLING);				// might need to be a rising signal.  TBD
	timeperiod = millis();
	tickold1 = tick1;
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (millis() > timeperiod + refreshrate) {
	timeperiod = millis();

		cadencefunction();
		gearshifter();
		cadenceleds();
		//  remote control tuning();                                             // well, should be obvious
		printing();
	
	}

}

void gearshifter() {
	if (rpm1 >= 20) {															// if pedaling at least an rpm of 20 check to see if gear shifting needs to happen.  
		
		if (rpm1 > (goalrpm + gearrange)) {
			if (currentgear == 11) { return; }
			currentgear++;												// shift harder
		}

		if (rpm1 < (goalrpm - gearrange)) {
			if (currentgear == 1) { return; }
			currentgear--;															//shift easier
		}


	}

	

}

void cadencefunction()   // cadence calculations and LEDS
{

	tickdiff1 = tick1 - tickold1;
	tickold1 = tick1;
	rpm1 = conversiontick1 * tickdiff1 / refreshrate;  // refreshrate is coming out to be 2001millis which shouldnt really effect the calculation much
}
void cadenceleds()
{
		digitalWrite(LED1, LOW);
		digitalWrite(LED2, LOW);
		digitalWrite(LED3, LOW);
		digitalWrite(LED4, LOW);
		digitalWrite(LED5, LOW);

		switch (rpm1) {
		case 0 ... 2:
			digitalWrite(LED1, HIGH);
			break;
		case 3 ... 4:
			digitalWrite(LED2, HIGH);
			break;
		case 5 ...7:
			digitalWrite(LED3, HIGH);
			break;
		case 8 ... 9:
			digitalWrite(LED4, HIGH);
			break;
		case 10 ... 100:
			digitalWrite(LED5, HIGH);
			break;
		}

		timeperiod = millis();

	
}

void printing()
{
	Serial.print("time = ");
	int timenow = (millis() / 1000);
		Serial.println(timenow);

		Serial.print(" ticker count is: ");
		Serial.println(tick1);
		//Serial.println(tickdiff1);


		Serial.print("RPM is ");
		Serial.println(rpm1);
		Serial.print("cassete count = ");
		Serial.println(tick2);
		//Serial.print("revoultion count = ");
		//Serial.println(wcount);
		Serial.print("ratio = ");
		Serial.println(ratio);
		Serial.println(" ");
}










// interrupt counters are only counting
void ticking1()
{
	tick1++ ;
	//digitalWrite(LED5, HIGH); //should be removed from code once finalized and re add in functionality of 5th LED
}
 
void ticking2()
{
	tick2++;
	//digitalWrite(LED5, HIGH);

}

void Wrev()
{
	wcount++;
}

















// ------------------------------------------------------------------------------ below line the fucntions are not being called









void gearratiofunction() //calculate gear ratio depending on tick1 and tick 2.  easier on a 1x drivetrain.
						 //need to calculate the ratios, and find the ranges to set to LCD? screen as the output.   
{
	tickdiff2 = tick2 - tickold2;
	tickold2 = tick2;
	ratio = tickdiff1 / tickdiff2;
	


}

void speedcalcs()
{

}




