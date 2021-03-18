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

/* Pin 1 is PB
Pin 3,4,5,6,7... etc are LEDs. */


#include "ThingSpeak.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "degroot"; 
const char* password = "degroot143!";

#define channelid 1038676                                 
#define apikey_write "GYYOAQPPZ8HGU5LM"               

unsigned long channel1 = channelid;
const char * apikey = apikey_write;

const char* host = "maker.ifttt.com";
const int httpsPort = 443;

float PH;
int stopping=0;
WiFiClient  espclient;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup() 
{
	//LED Setup 
	pinMode(3, OUTPUT); //LED1 RGB Sensor
	pinMode(4, OUTPUT); //LED2 Wifi
	pinMode(5, OUTPUT); //LED3 IFTTT Host
	pinMode(6, OUTPUT); //LED4 Processing
	//TEST
	digitalWrite(3, HIGH);
	digitalWrite(4, HIGH);
	digitalWrite(5, HIGH);
	digitalWrite(6, HIGH);
	delay(1000);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);
	digitalWrite(6, LOW);
	
	//Input Setup
	pinMode(1, INPUT) 
	
	Serial.begin(115200);
	delay(100);
	Serial.println("");
	Serial.println("INITALIZED SERIAL CONNECTION");
	
	if (tcs.begin()) 
	{
		Serial.println("Found sensor");
		digitalWrite(3, HIGH);
	} 
	else 
	{
		Serial.println("No TCS34725 found ... check your connections");
		while (1);
	}
	Serial.print("Connecting To: ");
	Serial.println(ssid);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(750);
		Serial.printlm("NOT CONNECTED TO WIFI");
	}
	Serial.println("");
	digitalWrite(4, HIGH);
	Serial.println("WiFi Connected.");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());  

	ThingSpeak.begin(espclient);
	
	WiFiClientSecure client;
	Serial.print("connecting to ");
	Serial.println(host);
	if (!client.connect(host, httpsPort)) 
	{
		Serial.println("connection failed");
		return;
	}
	
	digitalWrite(5, HIGH);
}

void loop() 
{
	uint16_t colorTemp;
	digitalWrite(6, HIGH);
	while (digitalRead()==HIGH)
	{
		digitalWrite(6, LOW);
		Serial.println("Button Pressed, Reading Data");
		colorTemp = collectData();
		Serial.println("Data: " + ColorTemp);
		Storedata(colorTemp);
		if(colorTemp > 4500 && colorTemp < 6800)
		{
			Hydrated(colorTemp);
		}
		/* if(colorTemp <= 4500)
		{
			DeHydrated(colorTemp);
		}
		if(colorTemp >= 6800)
		{
			ReadError(colorTemp);
		} */
		delay(5000)
	}
	/* if(stopping ==1) 
	{
		
	}
	else 
	{
		delay(1000);
		Serial.println("Align Strip For Reading");
		delay(8000);
		Serial.println("Reading in Progress");
		conversiontoPH();
	} */
}

uint16_t collectData(void)
{
	uint16_t r, g, b, c, colorTemp;
	tcs.getRawData(&r, &g, &b, &c);
	colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
	return colorTemp;
}

void Storedata(uint16_t colorTemp)
{
	uint16_t colorTemp
	String url1 = "/trigger/Flush/with/key/";

	client.print(String("GET ") + url1 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Data: " + colorTemp + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n");

	Serial.println("request sent");
	while (client.connected()) 
	{
		String line = client.readStringUntil('\n');
		if (line == "\r") 
		{
			Serial.println("headers received");
			break;
		}
	}
	String line = client.readStringUntil('\n');

	if (line=="")
	{
		Serial.println("reply was:");
		Serial.println(line);
		Serial.println("Success");
	}
	else
	{
		Serial.println("reply was:");
		Serial.println(line);
		Serial.println("No A Success");
	}
}

void Hydrated(uint16_t colorTemp)
{
	uint16_t colorTemp
	String url1 = "/trigger/Flush/with/key/";
	String url2 = "/trigger/Flush/with/key/";

	client.print(String("GET ") + url1 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Data: " + colorTemp + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n");

	Serial.println("request sent");
	while (client.connected()) 
	{
		String line = client.readStringUntil('\n');
		if (line == "\r") 
		{
			Serial.println("headers received");
			break;
		}
	}
	String line = client.readStringUntil('\n');

	if (line=="")
	{
		Serial.println("reply was:");
		Serial.println(line);
		Serial.println("Success");
	}
	else
	{
		Serial.println("reply was:");
		Serial.println(line);
		Serial.println("No A Success");
	}

	client.print(String("GET ") + url2 + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Data: " + colorTemp + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n");

	Serial.println("request sent");
	while (client.connected()) 
	{
		String line = client.readStringUntil('\n');
		if (line == "\r") 
		{
			Serial.println("headers received");
			break;
		}
	}
	String line = client.readStringUntil('\n');

	if (line=="")
	{
		Serial.println("reply was:");
		Serial.println(line);
		Serial.println("Success");
	}
	else
	{
		Serial.println("reply was:");
		Serial.println(line);
		Serial.println("No A Success");
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
	if ((colorTemp < 0) || (colorTemp > 8000))
	{
		Serial.println("Value not found");
		loop(); 
	}
}