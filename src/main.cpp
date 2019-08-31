/*
main.cpp - Main of WeatherStation
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

#include <Arduino.h>
#include "Debug.h"
#include <WiFi.h>
#include "WeatherMQTT.h"
#include "RainSensor.h"
#include "esp_wifi.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "AOK5055.h"

//include secure networkSetup.h, if exist
#if __has_include("networkSetup.h")
#include "networkSetup.h"
#endif

#ifndef NETWORKSETUP_H
//Wifi
#define WIFI_SSID     "<your-SSID>"
#define WIFI_PASSWORD "<your-Password>"

// MQTT
#define MQTT_SERVER    "<your MQTT Server>"
#define MQTT_CLIENT_ID "WeatherClientID"
#define MQTT_USER      "<your MQTT user>"
#define MQTT_PASSWORD  "<your MQTT Password>"

#endif

// defaults
#define DEFAULT_SEND_CYCLE 60           // send every 3 Minutes
#define DEFAULT_RAIN_PER_TICK 0.47       // one tic =  0.47 mm
#define DEFAULT_WIND_SPEED_PER_TICK 2.4  //  one tic/second = 2.4 km/h
#define DEFAULT_RAIN_ALARM_VALUE 3850    // max 4096 = no rain (ADC) must calibrated
#define DEFAULT_WIND_ALARM_VALUE 15.0    // Alarm wenn Speed more then 15km/h
#define DEFAULT_ALTITUDE 505             // Altidue over NN för relative Pressure

#define PRESSURE_RELATIV_CORRECTION 63  // todo: als set realisieren

#define RAINSENSOR_PIN 39
#define RAINSENSOR_POWER 21    // Switch power on for meassurement
#define DEFAULT_RAINSENSOR_ALARM_RESET_TIME 600  //ms

// I2C, BME280, BH1750
#define WEATHER_I2C_SLC_PIN 19
#define WEATHER_I2C_SLA_PIN 18
#define WEATHER_BH1750_ADDR 0x23
#define BRIGHTNESS_FACTOR 1.44
// AOK5055
#define AOK_RAIN_PIN 27
#define AOK_RAIN_GPIO_PIN GPIO_NUM_27
#define AOK_POWER_PIN 33
#define AOK_WIND_SPEED_PIN 26
#define AOK_WIND_DIRECTION_PIN 25
#define DEFAULT_WIND_ALARM_RESET_TIME 600  //ms

//battery
#define BATTERY_PIN 36
#define BATTERY_LOW  2630  //ADC Calibrate by voltage divider (3.3V)
#define BATTERY_FULL 3737  //ADC Calibrate by voltage divider (4.2V)

WeatherMQTT  mqtt;
RainSensor   rainSensor;
BH1750 lightMeter(WEATHER_BH1750_ADDR);
Adafruit_BME280 bme;
AOK5055 aok;

boolean wifiConnect(){
  DEBUG_println("Connect Wifi  " );
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int i = 0;
  while ( WiFi.status() != WL_CONNECTED ) {
    DEBUG_print("." );
    delay(100);
    if (i++ > 30) {
      DEBUG_println("!");
      DEBUG_println("Wifi connection died!!!!");
      return false;
    }
  }
  DEBUG_println("");
  DEBUG_printf("Connection takes : %dms\n",i*100);
  DEBUG_print("Mac address: "); DEBUG_println(WiFi.macAddress());
  DEBUG_print("IP address: ");  DEBUG_println(WiFi.localIP());
  return true;
}

void checkBattery() {
  unsigned int batt = (int)analogRead(BATTERY_PIN);
  DEBUG_println("Battery : " + String(batt));
  if (batt <= BATTERY_LOW) mqtt.battery = 0;
  else mqtt.battery = (byte)(((batt - BATTERY_LOW) *100)/(BATTERY_FULL-BATTERY_LOW));
}


void goSleep(unsigned long sleepTime){
  unsigned long start = millis();
  DEBUG_println("Sleep");
  #ifdef DEBUG
    delay(500);
  #endif
  esp_sleep_enable_timer_wakeup((sleepTime-(millis()-start))*1000);
  esp_light_sleep_start();
  switch (esp_sleep_get_wakeup_cause()) {
    case ESP_SLEEP_WAKEUP_TIMER: {
      DEBUG_printf("Wake up from timer. Time spent in sleep: %ldms\n", millis()-start);
      break;
    }
    case ESP_SLEEP_WAKEUP_GPIO: { // Rain wipp
      Serial.println("Wake up from GPIO (Rain)");
        // go sleep for rest of time (Realtime)
      while(sleepTime > (millis()-start)) delay(5);
      break;
    }
      default: {
        DEBUG_println("Wake up from other.");
        break;
      }
  }
}

void readDefault() {
  if (!mqtt.restoreData()) {
    mqtt.cycle = DEFAULT_SEND_CYCLE;
    mqtt.windSpeedPerTic = DEFAULT_WIND_SPEED_PER_TICK;
    mqtt.rainPerTic = DEFAULT_RAIN_PER_TICK;
    mqtt.windAlarmValue = DEFAULT_WIND_ALARM_VALUE;
    mqtt.windAlarmResetTime = DEFAULT_WIND_ALARM_RESET_TIME;
    mqtt.rainAlarmValue = DEFAULT_RAIN_ALARM_VALUE;
    mqtt.rainAlarmResetTime = DEFAULT_RAINSENSOR_ALARM_RESET_TIME;
    mqtt.altitude = 505;
    mqtt.saveData();
  }
}

unsigned long oneMinute;
unsigned long cycleTime;
unsigned long cycleStartTime;
unsigned long startTime ;


void setup() {
  delay(1000); // Delay for load Elko's
  DEBUG_begin(115200);
  DEBUG_println("WeatherStation start in DEBUG Mode.");

  // bt not need
  esp_bt_controller_disable();
  esp_bluedroid_disable();

  readDefault();
  mqtt.begin(MQTT_SERVER,MQTT_CLIENT_ID,MQTT_USER,MQTT_PASSWORD);
  Wire.begin(WEATHER_I2C_SLA_PIN,WEATHER_I2C_SLC_PIN);
  //	i2cdetect();
  //BH1750
  	if (lightMeter.begin(BH1750::ONE_TIME_LOW_RES_MODE)) {
      DEBUG_println(("BH1750 Advanced begin"));
    }
    else {
      DEBUG_println("Error initialising BH1750");
    }
  // BME280
  	if (!bme.begin(0x76)) {
  			DEBUG_println("Could not find a valid BME280 sensor, check wiring!");
  	}
    else {
      DEBUG_println("Initialising BME280 OK");
    }
  rainSensor.begin(RAINSENSOR_PIN,RAINSENSOR_POWER,mqtt.rainAlarmValue);
  aok.begin(AOK_RAIN_PIN, AOK_POWER_PIN,AOK_WIND_SPEED_PIN, AOK_WIND_DIRECTION_PIN);

  gpio_wakeup_enable(AOK_RAIN_GPIO_PIN ,GPIO_INTR_HIGH_LEVEL);
  esp_sleep_enable_gpio_wakeup();
  mqtt.upTime = 0;
  oneMinute =  millis();
  cycleTime =  (unsigned long)mqtt.cycle * 1000;
  cycleStartTime = millis();
}


unsigned long rainAlarmTime,windAlarmTime;

void everyLoop() {
  boolean locAlarm=false;
  aok.startWindSpeed();
  mqtt.rain = aok.getRain(mqtt.rainPerTic);
  mqtt.windSpeedNow = aok.stopWindSpeed(mqtt.windSpeedPerTic);
  if (mqtt.windSpeedNow >= mqtt.windAlarmValue) {
      windAlarmTime = millis();
      if ( !mqtt.windAlarm) {
        locAlarm =true;
        mqtt.windAlarm = true;
      }
  }
  mqtt.rainSensor=rainSensor.read();
  if (mqtt.rainSensor <= mqtt.rainAlarmValue) {
    windAlarmTime = millis();
    if (!mqtt.rainAlarm) {
      locAlarm =true;
      mqtt.rainAlarm = true;
    }
  }
  if (locAlarm) {  // send Alarm Info
    esp_wifi_start();
    delay(500);
   if (wifiConnect()) {
       mqtt.sendAlarm();
    }
    esp_wifi_stop();
    delay(500);
    locAlarm = false;
  }
}

void cycleLoop() {
     // Todo: reset Alarm
     if ((mqtt.windAlarm) &&
        (millis()-windAlarmTime > mqtt.windAlarmResetTime*1000)) mqtt.windAlarm = false;
     if ((mqtt.rainAlarm) &&
         (millis()-rainAlarmTime > mqtt.rainAlarmResetTime*1000)) mqtt.rainAlarm = false;

     // getSensors();
     // aok.countOneMinute();
     checkBattery();
     byte windD = aok.getWindDirektion();
     if (windD < 16) mqtt.windDirection=windD;
     mqtt.rain1h  = aok.getRain1h(mqtt.rainPerTic);
     mqtt.rain24h = aok.getRain24h(mqtt.rainPerTic);
     mqtt.windSpeed = aok.getWindSpeed(mqtt.windSpeedPerTic);
     mqtt.windSpeedMax = aok.getWindSpeedMax(mqtt.windSpeedPerTic);
     mqtt.brightness=lightMeter.readLightLevel()*BRIGHTNESS_FACTOR;
     DEBUG_printf("Brightness : %d \n",mqtt.brightness);
     mqtt.temperature = bme.readTemperature();
     mqtt.humidity = bme.readHumidity();
     float Th = mqtt.temperature + 273.15;
     mqtt.pressure = bme.readPressure()/100.0F * pow( Th / (Th + (0.0065 * mqtt.altitude)), -5.255);
     // send MQTT
     esp_wifi_start();
     if (wifiConnect()) {
       mqtt.loop();
     }
     esp_wifi_stop();
}


byte loopCountMinute = 0;
int loopCountAll = 0;

void loop() {
  startTime = millis() ;
  DEBUG_printf("everyLoop------------------------- %lu\n\n",millis()-oneMinute);
  everyLoop();

 if (millis() - oneMinute >= 60000) {
   DEBUG_printf("One Minute Hardbeat____________________ %lu\n\n",millis()-oneMinute);
   oneMinute = millis();
   mqtt.upTime++;
   aok.countOneMinute();
 }

if (millis() - cycleStartTime >= cycleTime) {
   DEBUG_printf("Cycle Hardbeat_________________________ %lu\n",millis() - cycleStartTime);
   cycleStartTime = millis();
   cycleLoop();
   cycleTime =  (unsigned long)mqtt.cycle * 1000; //if Updated
 }

unsigned long neededTime =  millis()-startTime; // needed Time
DEBUG_printf("Aktive Loop needs : %lu ms\n\n",neededTime);
if (neededTime<10000) goSleep(10000-neededTime);
DEBUG_printf("Loop needs : %lu ms\n",millis()-startTime);
}
