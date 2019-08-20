/*
 * WeatherMQTT.h
 *
 *  Created on: 06.02.2019
 *      Author: stefan
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
	void publishAll();

public:
	unsigned short cycle;  // SendCycle in seconds
	float temperature;     // xx.x °C
	byte windDirection;    // "N","NNO","NO","ONO","O"..."NNW"; 0=N,15=NNW
	float windSpeed;       // Km/h Median from last 10 minutes
	float windSpeedPerTic;
	float windSpeedNow;    // KM/h Max from last 10 minutes
	float windSpeedMax;    // KM/h Max from last 10 minutes
	boolean windAlarm;     // true if windAlarmValue <= windSpeedMax
	float windAlarmValue;  // km/h
  float rain;            // mm(l)/h summe of last 3 Minute *20
	float rainPerTic;
	float rain1h;          // mm(l) of last h
	float rain24h;         // mm(l) of last 24h
	unsigned short rainSensor;      // 0..4096
	boolean rainAlarm;            //
	unsigned short rainAlarmValue;  // calibrate 0 (0..4096)
	byte humidity;                // 0..100%
	unsigned short pressure;        // mbar
	unsigned short brightness;      // 0..64000
	byte battery;         // 0..100% (0=3.3V 100=4.2V)

	WeatherMQTT();
	void begin(const char server[],const char* clientID,const char* user,const char* password);
	boolean connect();
	void sendAlarm();
	void saveData();
	boolean restoreData();
	void loop();

};

#endif /* WEATHERMQTT_H_ */
