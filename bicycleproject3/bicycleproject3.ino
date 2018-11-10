/*
 Name:		bicycleproject3.ino
 Created:	10/27/2018 8:26:58 AM
 Author:	brian
 Version 3: 

 

  check limits to current gear

*/


#include <PinChangeInt.h>
#include <IRremoteInt.h>
#include <IRremote.h>
#include<eeprom.h>
#include <LiquidCrystal.h>

#define ticker1 9    // opto interruptor on chainring


const int RECV_PIN = 12;	    //IR reciever pin
IRrecv irrecv(RECV_PIN);		//IR reciever pin
decode_results results;			//IR reciever pin


int idealcadence;
boolean flagfaster;
boolean flagslower;
boolean flagscreen;    // car to toggle between screens


volatile int tick1 = 0;  //cadence sensor
long timeperiod; // being used to compare to millis
int refreshrate = 2000;  //   how often it refreshes and recalculates the RPM
long screentime;  //time var to change screen between running mode and setting mode



//		ints for cadence function
unsigned long tickold1;
int tickdiff1;
unsigned int rpm1;
long int conversiontick1 = 500;   // conversionrate for tick 1			needs to be set


//			ints for gear shifter
int currentgear ;																// is set when power button is hit. resets last saved.
int cadencerange;																	// +- range before shifts occur




const int rs = 8, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long shiftdelay = 0;					// time int for shift delay

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	pinMode(ticker1, INPUT);

	
	attachPinChangeInterrupt(ticker1, ticking1, FALLING);


	tickold1 = tick1;

	irrecv.enableIRIn();
	irrecv.blink13(true);

	idealcadence = EEPROM.read(1);
	cadencerange = EEPROM.read(0);
	currentgear = EEPROM.read(2);

	lcd.begin(16,2);

	flagscreen = true;
	timeperiod = millis();
	screentime = millis();
}


void loop() 													//MAIN CODE
{	
if (millis() > timeperiod + refreshrate) {
		timeperiod = millis();

		cadencefunction();
		gearshifter();
		printing();
		LCDprinting();

	}
		remote();                                             
	
}


void LCDprinting() {
	switch (flagscreen) {
	case true:					//case where button has recently been pressed
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("set cadence: ");
		
		lcd.print(idealcadence);
		lcd.setCursor(0, 1);							// column 0, row 1
		lcd.print("cadence range:");
		lcd.print(cadencerange);
		break;

	case false:
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("cadence is: ");
			lcd.print(rpm1);
		lcd.setCursor(0, 1);
		lcd.print("current gear: ");
		lcd.print(currentgear);
		break;


	}
}



void gearshifter() {
	if (rpm1 > 2) {					// if pedaling at least an rpm of 20 check to see if gear shifting needs to happen.
		
		if (millis() > shiftdelay + 1000) {
			if (rpm1 > (idealcadence + cadencerange)) {
				if (currentgear == 11) { return; }
				currentgear++;	
				shiftdelay = millis();
			}

			if (rpm1 < (idealcadence - cadencerange)) {						//shift easier
				if (currentgear == 1) { return; }
				currentgear--;	
				shiftdelay = millis();
			}
			 
		}
	}

	

}


void cadencefunction()   // cadence calculations and LEDS
{

	tickdiff1 = tick1 - tickold1;
	tickold1 = tick1;
	rpm1 = conversiontick1 * tickdiff1 / refreshrate;  // refreshrate is coming out to be 2001millis which shouldnt really effect the calculation much
}


void printing()
{
	Serial.print("time = ");
	int timenow = (millis() / 1000);
		Serial.println(timenow);

		Serial.print(" ticker count is: ");
		Serial.println(tick1);
		//Serial.println(tickdiff1);


		Serial.print("rpm1 is ");
		Serial.println(rpm1);
		//Serial.print("cassete count = ");
		//Serial.println(tick2);
		//Serial.print("revoultion count = ");
		//Serial.println(wcount);
		//Serial.print("ratio = ");
		//Serial.println(ratio);
		//Serial.println(results.value, HEX);
		Serial.print("ideal cadence is ");
		Serial.println(idealcadence);
		Serial.print("gearrange is ");
		Serial.println(cadencerange);
		Serial.print("current gear is ");
		Serial.println(currentgear);
		
		Serial.println(" ");
}


void remote() {
	if (irrecv.decode(&results)) {
		Serial.println(results.value, HEX);
		irrecv.resume();
		flagscreen = true;										//adjustment screen
	screentime = millis();
	}

	if (millis() > screentime + 5000) {
		flagscreen = false;
	}



	if (((results.value == 0xFF629D ) || ((results.value == 0xFFFFFFFF) && (flagfaster == true))) && idealcadence < 99)
	{
		idealcadence++;
		flagfaster = true;
		flagslower = false;
	}
	
	if (((results.value == 0xFFA857) || ((results.value == 0xFFFFFFFF) && (flagslower == true))) && idealcadence > 1)
	{
		idealcadence--;
		flagslower = true;
		flagfaster = false;
	}
	


	switch (results.value)
	{
	case 0xFF6897:
		cadencerange = 10;
		break;
	case 0xFF52AD:
		cadencerange = 9;
		break;
	case 0xFF4AB5:
		cadencerange = 8;
		break;
	case 0xFF42BD:
		cadencerange = 7;
		break;
	case 0xFF5AA5:
		cadencerange = 6;
		break;
	case 0xFF38C7:
		cadencerange = 5;
		break;
	case 0xFF10EF:
		cadencerange = 4;
		break;
	case 0xFF7A85:
		cadencerange = 3;
		break;
	case 0xFF18E7:
		cadencerange = 2;
		break;
	case 0xFF30CF:
		cadencerange = 1;
		break;



	case 0xFFA25D :									// powerbutton saves current gear range and ideal cadence to be used upon bootup
		EEPROM.write(0, cadencerange);
		EEPROM.write(1, idealcadence);
		EEPROM.write(2, currentgear);
		break;

	case 0xFFE21D :							//func/stop button resets idealcadence and candence range to a default
		idealcadence = 70;
		cadencerange = 6;
		break;

	case 0xFF9867:							// eq button sets a good testing range
		idealcadence = 6;
		cadencerange = 2;
		break;
	}


		results.value = 0x000000;
}


void ticking1()
{
	tick1++ ;
}
 














