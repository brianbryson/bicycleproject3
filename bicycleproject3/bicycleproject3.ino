/*
 Name:		bicycleproject3.ino
 Created:	10/27/2018 8:26:58 AM
 Author:	brian
*/

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.println("hello world");
	
	int timenow = millis();
	Serial.println(timenow);
	delay(100);
}
