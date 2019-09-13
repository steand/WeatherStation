/*
WeatherMQTT.h - Include file for class "WeatherMQTT".
Copyright (C) 2019  by Stefan Andres

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef WEATHERMQTT_H_
#define WEATHERMQTT_H_

#include "Arduino.h"
#include <WiFi.h>
#include <MQTT.h>


class WeatherMQTT {
private:
	WiFiClient wifiClient;
	MQTTClient client;
	const char* clientID;
	const char* user;
	const char* password;

	void pub(String tag,String Value);
	void pub(String tag,float value);
	void pub(String tag,byte value);
	void pub(String tag,unsigned short value);
	void pub(String tag,boolean value);
	void pub(String tag,long value);
	void publishAll();

public:
	unsigned long  upTime ; // Uptime in Minutes
	unsigned short cycle;  // SendCycle in seconds
	float temperature;     // xx.x °C
	byte windDirection;    // "N","NNO","NO","ONO","O"..."NNW"; 0=N,15=NNW
	float windSpeed;       // Km/h Median from last 10 minutes
	float windSpeedPerTic;
	float windSpeedNow;    // KM/h Max from last 10 minutes
	float windSpeedMax;    // KM/h Max from last 10 minutes
	boolean windAlarm;     // true if windAlarmValue <= windSpeedMax
	float windAlarmValue;  // km/h
	unsigned short windAlarmResetTime; // time in millis
  float rain;            // mm(l)/h summe of last 3 Minute *20
	float rainPerTic;
	float rain1h;          // mm(l) of last h
	float rain24h;         // mm(l) of last 24h
	unsigned short rainSensor;      // 0..4096
	boolean rainAlarm;            //
	unsigned short rainAlarmValue;  // calibrate 0 (0..4096)
	unsigned short rainAlarmResetTime; // time in millis
	byte humidity;                // 0..100%
	unsigned short pressure;        // mbar
	unsigned short altitude;      //altitude over NN
	unsigned short brightness;      // 0..64000
	float battery;               // voltage
	float solarVolt;
	long wifiRSSI;

	WeatherMQTT();
	void begin(const char server[],const char* clientID,const char* user,const char* password);
	boolean connect();
	void sendAlarm();
	void saveData();
	boolean restoreData();
	void loop();

};

#endif /* WEATHERMQTT_H_ */
