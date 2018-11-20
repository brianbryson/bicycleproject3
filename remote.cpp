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



	if (((results.value == 0xFF629D) || ((results.value == 0xFFFFFFFF) && (flagfaster == true))) && idealcadence < 99)
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



	case 0xFFA25D:									// powerbutton saves current gear range and ideal cadence to be used upon bootup
		EEPROM.write(0, cadencerange);
		EEPROM.write(1, idealcadence);
		EEPROM.write(2, currentgear);
		break;

	}


	results.value = 0x000000;
}
