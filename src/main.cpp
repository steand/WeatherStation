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

#include "networkSetup.h"  //my secure networkSetup (no GitHub)
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
#define DEFAULT_SEND_CYCLE 60            // send every 180s
#define DEFAULT_RAIN_PER_TICK 0.47       // one tic =  0.47 mm
#define DEFAULT_WIND_SPEED_PER_TICK 2.4  //  one tic/second = 2.4 km/h
#define DEFAULT_RAIN_ALARM_VALUE 4000    // max 4096 = no rain (ADC) must calibrated
#define DEFAULT_WIND_ALARM_VALUE 15.0    // Alarm wenn Speed more then 15km/h
#define DEFAULT_ALTITUDE 505             // Altidue over NN för relative Pressure

#define PRESSURE_RELATIV_CORRECTION 63  // todo: als set realisieren

#define RAINSENSOR_PIN 35
#define RAINSENSOR_POWER 32    // Switch power on for meassurement

// I2C, BME280, BH1750
#define WEATHER_I2C_SLC_PIN 33
#define WEATHER_I2C_SLA_PIN 25
#define WEATHER_BH1750_ADDR 0x23
#define BRIGHTNESS_FACTOR 1.44
// AOK5055
#define AOK_RAIN_PIN 26
#define AOK_RAIN_GPIO_PIN GPIO_NUM_26
#define AOK_POWER_PIN 27
#define AOK_WIND_SPEED_PIN 12
#define AOK_WIND_DIRECTION_PIN 14

//battery
#define BATTERY_PIN 39
#define BATTERY_LOW  2475  //ADC Calibrate by voltage divider (3.3V)
#define BATTERY_FULL 3280  //ADC Calibrate by voltage divider (4.2V)

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
  DEBUG_print("IP address: "); DEBUG_println(WiFi.localIP());
  return true;
}

void checkBattery() {
  unsigned int batt = (int)analogRead(BATTERY_PIN);
  DEBUG_println("Battery : " + String(batt));
  if (batt <= BATTERY_LOW) mqtt.battery = 0;
  else {
    if (batt >= BATTERY_FULL) mqtt.battery = 100;
    else mqtt.battery = (byte)(((batt - BATTERY_LOW) *100)/(BATTERY_FULL-BATTERY_LOW));
  }
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
//  if (!mqtt.restoreData()) {
    mqtt.cycle = DEFAULT_SEND_CYCLE;
    mqtt.windSpeedPerTic = DEFAULT_WIND_SPEED_PER_TICK;
    mqtt.rainPerTic = DEFAULT_RAIN_PER_TICK;
    mqtt.windAlarmValue = DEFAULT_WIND_ALARM_VALUE;
    mqtt.rainAlarmValue = DEFAULT_RAIN_ALARM_VALUE;
    mqtt.saveData();
//  }
}


void setup() {
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
}



void everyLoop() {
  boolean locAlarm=false;
  aok.startWindSpeed();
  mqtt.rain = aok.getRain(mqtt.rainPerTic);
  mqtt.windSpeedNow = aok.stopWindSpeed(mqtt.windSpeedPerTic);
  if ( !mqtt.windAlarm && (mqtt.windSpeedNow >= mqtt.windAlarmValue)) {
      mqtt.windAlarm = true;
      locAlarm =true;
  }
  mqtt.rainSensor=rainSensor.read();
  if (!mqtt.rainAlarm && rainSensor.getAlarm()) {
    mqtt.rainAlarm = true;
    locAlarm =true;
  }
  if (locAlarm) {  // send Alarm Info
    esp_wifi_start();
    if (wifiConnect()) {
      mqtt.sendAlarm();
    }
    esp_wifi_stop();
    locAlarm = false;
  }
}

void cycleLoop() {
     // Todo: reset Alarm

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
     DEBUG_println("Brightness : " + String(mqtt.brightness));
     mqtt.temperature = bme.readTemperature();
     mqtt.humidity = bme.readHumidity();
     mqtt.pressure = (bme.readPressure()/100) + PRESSURE_RELATIV_CORRECTION;
     // send MQTT
     esp_wifi_start();
     if (wifiConnect()) {
       mqtt.loop();
     }
     esp_wifi_stop();
}


byte loopCountMinute = 0;
int loopCountAll = 0;

unsigned long oneMinute = 0;
unsigned long cycleTime = 0;
unsigned long startTime = 0;

void loop() {
  startTime = millis() ;
  DEBUG_println("everyLoop-------------------------:" + String(oneMinute));
  everyLoop();

 if (oneMinute >= 60000) {
   DEBUG_println("1 Minute Hardbeat #"+ String(oneMinute));
   oneMinute = 0 ;
   aok.countOneMinute();
 }

if (cycleTime >= (unsigned long)mqtt.cycle * 1000) {
   DEBUG_println("Cycle Hardbeat_________________________:"+ String(cycleTime));

   cycleTime =  0;
   cycleLoop();
 }

unsigned long neededTime =  millis()-startTime; // needed Time

goSleep(10000-neededTime);
oneMinute  += millis()-startTime;
cycleTime  += millis()-startTime;
DEBUG_println("Loop needs : "+ String(millis()-startTime) + "ms");

}
