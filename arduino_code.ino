//Title: ThingSpeak.h
//Author: The MathWorks, Inc 
//Date: 2018
//Availability: https://github.com/mathworks/thingspeak-arduino/blob/master/src/ThingSpeak.h  

//Title: Adafruit_TCS34725.h
//Author: Kevin (KTOWN) Townsend for Adafruit Industries
//Date: 2013
//Availability: https://github.com/adafruit/Adafruit_TCS34725 

//Title: ESP8266Wifi.h
//Author: Ivan Grokhotkov 
//Date: 2014
//Availability: https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h 

//Title: RSSI_to_ThingSpeak.ino
//Author: Hans Scharler
//Date: 2019
//Availability: https://github.com/nothans/thingspeak-esp-examples/blob/master/examples/RSSI_to_ThingSpeak.ino


#include "ThingSpeak.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <ESP8266WiFi.h>

const char* ssid = "degroot"; 
const char* password = "degroot143!";

#define channelid 1038676                                 
#define apikey_write "GYYOAQPPZ8HGU5LM"               

unsigned long channel1 = channelid;
const char * apikey = apikey_write;

float PH;
int stopping=0;
WiFiClient  espclient;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() 
{
	Serial.begin(115200);
	delay(15000);
	Serial.println("");
	Serial.println("");
	Serial.print("Connecting To: ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print("*");
	}
	Serial.println("");
	Serial.println("WiFi Connected.");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());  

	ThingSpeak.begin(espclient);

	if (tcs.begin()) 
	{
		Serial.println("Found sensor");
	} 
	else 
	{
		Serial.println("No TCS34725 found ... check your connections");
		while (1);
	}
}

void loop() 
{
	if(stopping ==1) 
	{
		
	}
	else 
	{
	delay(1000);
	Serial.println("Align Strip For Reading");
	delay(8000);
	Serial.println("Reading in Progress");
	conversiontoPH();
	}
}


void conversiontoPH(void)
{
	uint16_t r, g, b, c, colorTemp;
	tcs.getRawData(&r, &g, &b, &c);
	colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
	if ((colorTemp > 0) && (colorTemp < 8000))
	{
		PH=2.7;
		int httpCode = ThingSpeak.writeField(channel1, 1, PH, apikey);

		if (httpCode == 200) 
		{
			Serial.println("Channel write successful.");
		}
		else 
		{
		Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
		}  
	stopping=1; 
	}
	if ((colorTemp < 0) || (colorTemp > 8000)){
	Serial.println("Value not found");
	loop(); 
	}
}
