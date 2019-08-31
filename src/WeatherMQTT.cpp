/*
WeatherMQTT.cpp - This class implement the handler for MQTT.
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

#include "WeatherMQTT.h"
#include <MQTT.h>
#include "Debug.h"
#include <EEPROM.h>

#define MAXTRY 5 // max try's for connection

#define MQTT_ROOT "WeatherStation/"

#define TAG_STATE                 MQTT_ROOT "State"
#define TAG_UPTIME                MQTT_ROOT "UpTime"
#define TAG_SEND_CYCLE            MQTT_ROOT "SendCycle"
#define TAG_SEND_CYCLE_SET        MQTT_ROOT "SendCycle/set"

#define TAG_TEMPERATURE           MQTT_ROOT "Temperature"
//Wind
#define TAG_WIND_DIRECTION        MQTT_ROOT "Wind/Direction"
#define TAG_WIND_SPEED            MQTT_ROOT "Wind/Speed"
#define TAG_WIND_SPEED_NOW        MQTT_ROOT "Wind/Speed/Now"
#define TAG_WIND_SPEED_PER_TIC     MQTT_ROOT "Wind/Speed/PerTic"
#define TAG_WIND_SPEED_PER_TIC_SET MQTT_ROOT "Wind/Speed/PerTic/set"
#define TAG_WIND_SPEED_MAX        MQTT_ROOT "Wind/Speed/Max"
#define TAG_WIND_ALARM            MQTT_ROOT "Wind/Alarm"
#define TAG_WIND_ALARM_VALUE      MQTT_ROOT "Wind/Alarm/Value"
#define TAG_WIND_ALARM_VALUE_SET  MQTT_ROOT "Wind/Alarm/Value/set"
#define TAG_WIND_ALARM_RESET      MQTT_ROOT "Wind/Alarm/ResetTime"
#define TAG_WIND_ALARM_RESET_SET  MQTT_ROOT "Wind/Alarm/ResetTime/set"
// Rain
#define TAG_RAIN                  MQTT_ROOT "Rain"
#define TAG_RAIN_PER_TIC          MQTT_ROOT "Rain/PerTic"
#define TAG_RAIN_PER_TIC_SET      MQTT_ROOT "Rain/PerTic/set"
#define TAG_RAIN_1H               MQTT_ROOT "Rain/1h"
#define TAG_RAIN_24H              MQTT_ROOT "Rain/24h"
// Rain Sensor
#define TAG_RAIN_SENSOR           MQTT_ROOT "Rain/Sensor"
#define TAG_RAIN_ALARM            MQTT_ROOT "Rain/Alarm"
#define TAG_RAIN_ALARM_VALUE      MQTT_ROOT "Rain/Alarm/Value"
#define TAG_RAIN_ALARM_VALUE_SET  MQTT_ROOT "Rain/Alarm/Value/set"
#define TAG_RAIN_ALARM_RESET      MQTT_ROOT "Rain/Alarm/ResetTime"
#define TAG_RAIN_ALARM_RESET_SET  MQTT_ROOT "Rain/Alarm/ResetTime/set"
// const char *TAG_RAIN_ALARM_SET     =   MQTT_ROOT"Rain/Alarm/Value/set";
// Environment
#define TAG_HUMIDITY              MQTT_ROOT "Humidity"
#define TAG_PRESSURE              MQTT_ROOT "Pressure"
#define TAG_PRESSURE_ALTITUDE     MQTT_ROOT "Pressure/Altitude"
#define TAG_PRESSURE_ALTITUDE_SET MQTT_ROOT "Pressure/Altitude/set"
#define TAG_BRIGHTNESS            MQTT_ROOT "Brightness"
#define TAG_BATTERY               MQTT_ROOT "Battery"
#define TAG_SOLARVOLT             MQTT_ROOT "SolarVoltage"

const char *direction[] = {"N","NNO","NO","ONO","O","OSO","SO","SSO",
                           "S","SSW","SW","WSW","W","WNW","NW","NNW"} ;



#define EEPROM_SIZE 22
byte    locCycle;
#define EEPROM_CYCLE_ADDR 1
#define LOC_CYCLE_MIN 60
#define LOC_CYCLE_MAX 300
float   locWindSpeedPerTic;
#define EEPROM_WIND_SPEED_PER_TIC_ADDR 2
#define LOC_WIND_SPEED_PER_TIC_MIN 2.0
#define LOC_WIND_SPEED_PER_TIC_MAX 4.0
float   locRainPerTic;
#define EEPROM_RAIN_PER_TIC_ADDR 6
#define LOC_RAIN_PER_TIC_MIN 2.0
#define LOC_RAIN_PER_TIC_MAX 4.0
float   locWindAlarmValue;
#define EEPROM_WIND_ALARM_VALUE_ADDR 10
#define LOC_WIND_ALARM_VALUE_MIN 1.0
#define LOC_WIND_ALARM_VALUE_MAX 50.0
unsigned short locWindAlarmReset;
#define EEPROM_WIND_ALARM_RESET_ADDR 14
unsigned short locRainAlarmValue;
#define EEPROM_RAIN_ALARM_VALUE_ADDR 16
#define LOC_RAIN_ALARM_VALUE_MIN 3500
#define LOC_RAIN_ALARM_VALUE_MAX 4096
unsigned short locRainAlarmReset;
#define EEPROM_RAIN_ALARM_RESET_ADDR 18
//pressure
unsigned short locPressureAtlitude;
#define EEPROM_PRESSURE_ALTITUDE_ADDR 20
#define LOC_PRESSURE_ALTITUDE_MAX 8848  //Mount Everest ;-))

boolean isResived = true; // First Time delete all set's from Broker

void callback(String &topic, String &payload) {
  isResived = true;
  DEBUG_print("Callback resived:");
  DEBUG_print(topic);
  DEBUG_print("=");
  DEBUG_println(payload);
  if (topic.equals(TAG_SEND_CYCLE_SET)) {
    locCycle = payload.toInt();
  }
  if (topic.equals(TAG_WIND_SPEED_PER_TIC_SET)) {
    locWindSpeedPerTic = payload.toFloat();
  }
  if (topic.equals(TAG_RAIN_PER_TIC_SET)) {
    locRainPerTic = payload.toFloat();
  }
  if (topic.equals(TAG_WIND_ALARM_VALUE_SET)) {
    locWindAlarmValue = payload.toInt();
  }
  if (topic.equals(TAG_WIND_ALARM_RESET_SET)) {
    locWindAlarmReset = payload.toInt();
  }
  if (topic.equals(TAG_RAIN_ALARM_VALUE_SET)) {
    locRainAlarmValue = payload.toInt();
  }
  if (topic.equals(TAG_RAIN_ALARM_RESET_SET)) {
    locRainAlarmReset = payload.toInt();
  }
  if (topic.equals(TAG_PRESSURE_ALTITUDE_SET)) {
    locPressureAtlitude = payload.toInt();
  }
}

WeatherMQTT::WeatherMQTT() {
}


void WeatherMQTT::saveData() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.writeByte(0,'I');
  EEPROM.writeByte(EEPROM_CYCLE_ADDR,this->cycle);
  EEPROM.writeFloat(EEPROM_WIND_SPEED_PER_TIC_ADDR,this->windSpeedPerTic);
  EEPROM.writeFloat(EEPROM_RAIN_PER_TIC_ADDR,this->rainPerTic);
  EEPROM.writeByte(EEPROM_WIND_ALARM_VALUE_ADDR,this->windAlarmValue);
  EEPROM.writeUShort(EEPROM_WIND_ALARM_RESET_ADDR, this->windAlarmResetTime);
  EEPROM.writeUShort(EEPROM_RAIN_ALARM_VALUE_ADDR, this->rainAlarmValue);
  EEPROM.writeUShort(EEPROM_RAIN_ALARM_RESET_ADDR, this->rainAlarmResetTime);
  EEPROM.writeUShort(EEPROM_PRESSURE_ALTITUDE_ADDR, this->altitude);
  EEPROM.commit();
  EEPROM.end();
}

boolean WeatherMQTT::restoreData() {
  EEPROM.begin(EEPROM_SIZE);
  if (EEPROM.readByte(0) == 'I') {
    this->cycle = EEPROM.readByte(EEPROM_CYCLE_ADDR);
    this->windSpeedPerTic=EEPROM.readFloat(EEPROM_WIND_SPEED_PER_TIC_ADDR);
    this->rainPerTic=EEPROM.readFloat(EEPROM_RAIN_PER_TIC_ADDR);
    this->windAlarmValue=EEPROM.readByte(EEPROM_WIND_ALARM_VALUE_ADDR);
    this->windAlarmResetTime=EEPROM.readUShort(EEPROM_WIND_ALARM_RESET_ADDR);
    this->rainAlarmValue=EEPROM.readUShort(EEPROM_RAIN_ALARM_VALUE_ADDR);
    this->rainAlarmResetTime=EEPROM.readUShort(EEPROM_RAIN_ALARM_RESET_ADDR);
    this->altitude=EEPROM.readUShort(EEPROM_PRESSURE_ALTITUDE_ADDR);
    EEPROM.end();
    return true;
  }
  EEPROM.end();
  return false;
}


void WeatherMQTT::begin(const char *server,const char* clientID,const char* user,const char* password) {
  this->clientID=clientID;
  this->user=user;
  this->password=password;

  client.begin(server,wifiClient);
  client.onMessage(callback);
  client.setWill(TAG_STATE,"offline",true,0);
  client.setOptions(this->cycle+10, true, 1000);
  locCycle = this->cycle;
  locWindSpeedPerTic = this->windSpeedPerTic;
  locRainPerTic = this->rainPerTic;
  locWindAlarmValue = this->windAlarmValue;
  locWindAlarmReset = this->windAlarmResetTime;
  locRainAlarmValue = this->rainAlarmValue;
  locRainAlarmReset = this->rainAlarmResetTime;
  locPressureAtlitude = this->altitude;
}


boolean WeatherMQTT::connect() {
  DEBUG_println("Connenct MQTT");


if (client.connect(this->clientID, this->user, this->password)) {
     DEBUG_println("publich state to  MQTT");
     client.publish(TAG_STATE,"online",true,0);
     client.subscribe(TAG_SEND_CYCLE_SET);
     client.subscribe(TAG_WIND_SPEED_PER_TIC_SET);
     client.subscribe(TAG_RAIN_PER_TIC_SET);
     client.subscribe(TAG_WIND_ALARM_VALUE_SET);
     client.subscribe(TAG_RAIN_ALARM_VALUE_SET);
     return true;
   }
  return false;
}


void WeatherMQTT::sendAlarm(){
     DEBUG_println("Send Alarm  MQTT");
     if (client.connect(this->clientID, this->user, this->password)) {
       client.publish(TAG_STATE,"online",true,0);
       pub(TAG_RAIN_SENSOR,this->rainSensor);
       pub(TAG_RAIN_ALARM,this->rainAlarm);
       pub(TAG_WIND_SPEED_MAX,this->windSpeedMax);
       pub(TAG_WIND_ALARM,this->windAlarm);
     }
     delay(500);
     wifiClient.flush();
}

void  WeatherMQTT::pub(String tag,String value) {
  client.publish(tag,value,true,0);
}

void WeatherMQTT::pub(String tag, float value) {
	pub(tag,String(value));
}
void WeatherMQTT::pub(String tag,byte value) {
	pub(tag,String(value));
}

void WeatherMQTT::pub(String tag,unsigned short value) {
  pub(tag,String(value));
}

void WeatherMQTT::pub(String tag,boolean value) {
  if (value) pub(tag,String("ON"));
  else pub(tag,String("OFF"));
}



void WeatherMQTT::publishAll() {
  char s[20];
  sprintf(s, "%lu:%02lu",this->upTime/60,this->upTime % 60);
  pub(TAG_UPTIME,String(s));
  pub(TAG_SEND_CYCLE,this->cycle);
  pub(TAG_TEMPERATURE,this->temperature);
  pub(TAG_WIND_DIRECTION,String(direction[this->windDirection]));
  pub(TAG_WIND_SPEED,this->windSpeed);
  pub(TAG_WIND_SPEED_NOW,this->windSpeedNow);
  pub(TAG_WIND_SPEED_PER_TIC,this->windSpeedPerTic);
  pub(TAG_WIND_SPEED_MAX,this->windSpeedMax);
  pub(TAG_WIND_ALARM,this->windAlarm);
  pub(TAG_WIND_ALARM_VALUE,this->windAlarmValue);
  pub(TAG_WIND_ALARM_RESET,this->windAlarmResetTime);

  pub(TAG_RAIN,this->rain);
  pub(TAG_RAIN_PER_TIC,this->rainPerTic);
  pub(TAG_RAIN_1H,this->rain1h);
  pub(TAG_RAIN_24H,this->rain24h);
  pub(TAG_RAIN_SENSOR,this->rainSensor);
  pub(TAG_RAIN_ALARM,this->rainAlarm);
  pub(TAG_RAIN_ALARM_VALUE,this->rainAlarmValue);
  pub(TAG_RAIN_ALARM_RESET,this->rainAlarmResetTime);

  pub(TAG_HUMIDITY,this->humidity);
  pub(TAG_PRESSURE,this->pressure);
  pub(TAG_PRESSURE_ALTITUDE,this->altitude);
  pub(TAG_BRIGHTNESS,this->brightness);
  pub(TAG_BATTERY,this->battery);
  pub(TAG_SOLARVOLT,this->solarVolt);
}


void WeatherMQTT::loop() {
  if (client.connect(this->clientID, this->user, this->password)) {
      DEBUG_println("MQTT-Loop: publich state");
      client.publish(TAG_STATE,"online",true,0);
      client.subscribe(TAG_SEND_CYCLE_SET);
      client.subscribe(TAG_WIND_SPEED_PER_TIC_SET);
      client.subscribe(TAG_RAIN_PER_TIC_SET);
      client.subscribe(TAG_WIND_ALARM_VALUE_SET);
      client.subscribe(TAG_WIND_ALARM_RESET_SET);
      client.subscribe(TAG_RAIN_ALARM_VALUE_SET);
      client.subscribe(TAG_RAIN_ALARM_RESET_SET);
      client.subscribe(TAG_PRESSURE_ALTITUDE_SET);
      client.loop();
      if (isResived) {
        if (((locCycle) != this->cycle) &&
            (locCycle >= LOC_CYCLE_MIN) &&
            (locCycle <= LOC_CYCLE_MAX)) {
            this->cycle = locCycle;
            client.setOptions(this->cycle+10, true, 1000);
        }
        if ((locWindSpeedPerTic != this->windSpeedPerTic) &&
            (locWindSpeedPerTic >= LOC_WIND_SPEED_PER_TIC_MIN) &&
            (locWindSpeedPerTic <= LOC_WIND_SPEED_PER_TIC_MAX)) {
            this->windSpeedPerTic = locWindSpeedPerTic;
        }
        if ((locRainPerTic != this->rainPerTic) &&
            (locRainPerTic >= LOC_RAIN_PER_TIC_MIN) &&
            (locRainPerTic <= LOC_RAIN_PER_TIC_MAX)) {
              this->rainPerTic = locRainPerTic;
        }
        if ((locWindAlarmValue != this->windAlarmValue) &&
            (locWindAlarmValue >= LOC_WIND_ALARM_VALUE_MIN) &&
            (locWindAlarmValue <= LOC_WIND_ALARM_VALUE_MAX)) {
              this->windAlarmValue = locWindAlarmValue;
        }
        if ((locWindAlarmReset != this->windAlarmResetTime)) {
          this->windAlarmResetTime = locWindAlarmReset;
        }
        if ((locRainAlarmValue != this->rainAlarmValue) &&
            (locRainAlarmValue >= LOC_RAIN_ALARM_VALUE_MIN) &&
            (locRainAlarmValue <= LOC_RAIN_ALARM_VALUE_MAX)) {
              this->rainAlarmValue = locRainAlarmValue;
        }
        if ((locRainAlarmReset != this->rainAlarmResetTime)) {
          this->rainAlarmResetTime = locRainAlarmReset;
        }
        if ((locPressureAtlitude != this->altitude) &&
            (locPressureAtlitude >= 0) &&
            (locPressureAtlitude <= LOC_PRESSURE_ALTITUDE_MAX)) {
              this->altitude = locPressureAtlitude;
        }
        // delete all sets from Broker
          pub(TAG_SEND_CYCLE_SET,String("")); // delete from Broker
          pub(TAG_WIND_SPEED_PER_TIC_SET,String("")); // delete from Broker
          pub(TAG_RAIN_PER_TIC_SET,String("")); // delete from Broker
          pub(TAG_WIND_ALARM_VALUE_SET,String("")); // delete from Broker
          pub(TAG_WIND_ALARM_RESET_SET,String("")); // delete from Broker
          pub(TAG_RAIN_ALARM_VALUE_SET,String("")); // delete from Broker
          pub(TAG_RAIN_ALARM_RESET_SET,String("")); // delete from Broker
          pub(TAG_PRESSURE_ALTITUDE_SET,String("")); // delete from Broker
          saveData();
          isResived = false;
      }
      publishAll();
      delay(500);
      wifiClient.flush();
    }
}
