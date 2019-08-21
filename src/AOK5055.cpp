/*
AOK5055.cpp - This class implement the AOK5055 sensors.
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
#include "AOK5055.h"
#include "Debug.h"


#define WIND_SPEED_MIN_MESSURE_TIME 1000
#define WIND_SPEED_ALARM_RESET 5.0  // Rest Alarm (windAlarmValue - 5.0)


// Rain handling
volatile unsigned long rainTicks;
volatile unsigned long lastRainTick;
unsigned long  rainCycle;           // Rain in the cycle
unsigned short rain1hArray[60];     // rain last 60 minutes
byte           rain1hIndex;
unsigned short rain24hArray[24];    // rain last day
byte           rain24hIndex;


void isrRain() {
  if ((millis()-lastRainTick) > 5) { // kill all bounds les 5 ms
    rainTicks++;
    lastRainTick = millis();
  }
}

// Wind handling

volatile byte windTicks;
volatile unsigned long lastWindTick;
unsigned long startWindTime;
volatile unsigned long  windSpeedArray[5];  // max Windspeed messure points
unsigned short windSpeedAverage[6];  // 1 Minute (6*10) times every Minute
unsigned short windSpeed10Average[10]; // 10 Minute  times every Minute
byte     windSpeedAverageIndex;
byte     windSpeed10AverageIndex;

unsigned long messMillis;

void IRAM_ATTR isrWind() {
   messMillis = millis();
  if ((messMillis-lastWindTick) > 5) { // kill all bounds les 5 ms
    if (windTicks < 3) windSpeedArray[windTicks] = messMillis;
      windTicks++;
      lastWindTick = messMillis;
  }
}


AOK5055::AOK5055() {
}

void AOK5055::begin(uint8_t rainPin, uint8_t powerPin, uint8_t windSpeedPin, uint8_t windDirectionPin) {
  byte i;
  this->rainPin = rainPin;
  this->powerPin = powerPin;
  this->windSpeedPin = windSpeedPin;
  this->windDirectionPin = windDirectionPin;
  rainTicks = 0;
  rainCycle = 0;
  for (i=0;i<60;i++) rain1hArray[i] = 0;     // tics last 60 minutes
  rain1hIndex=0;
  for (i=0;i<24;i++) rain24hArray[i]=0;    // tics last day
  rain24hIndex=0;
  for (i=0;i<6;i++) windSpeedAverage[i]=WIND_SPEED_MIN_MESSURE_TIME*2;
  for (i=0;i<10;i++) windSpeed10Average[i]=WIND_SPEED_MIN_MESSURE_TIME*2;
  windSpeedAverageIndex = 0;
  windSpeed10AverageIndex = 0;
  pinMode(rainPin,INPUT);
  pinMode(windSpeedPin,INPUT);
  pinMode(windDirectionPin,INPUT);
  pinMode(powerPin,OUTPUT);
  digitalWrite(this->powerPin, LOW); // Power off

   // Wind Trigger Pin Test only
  //  pinMode(4,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(this->rainPin),isrRain, FALLING);

}


unsigned long windStartTime;

void AOK5055::startWindSpeed() {
  windTicks=0;
// digitalWrite(4,HIGH);
  lastRainTick = millis();
  startWindTime=millis();
  attachInterrupt(digitalPinToInterrupt(this->windSpeedPin),isrWind, FALLING);
}

float AOK5055::stopWindSpeed(float windSpeedPerTic) {
  unsigned long windSpeedNow = 0;
  while (((millis() - startWindTime) <= WIND_SPEED_MIN_MESSURE_TIME) &&
           (windTicks < 3) ) delay(10);
  // digitalWrite(4,LOW);  // Test only
  detachInterrupt(digitalPinToInterrupt(this->windSpeedPin));
  DEBUG_println("WindSpeed needs (ms):" + String(millis() - startWindTime));
  // ToDo Speed berechnen
  windSpeedAverage[windSpeedAverageIndex] = WIND_SPEED_MIN_MESSURE_TIME*2;
  if (windTicks == 1 ) windSpeedAverage[windSpeedAverageIndex] = WIND_SPEED_MIN_MESSURE_TIME;
  if (windTicks == 2 ) windSpeedAverage[windSpeedAverageIndex] = windSpeedArray[1]-windSpeedArray[0];
  if (windTicks == 3 ) windSpeedAverage[windSpeedAverageIndex] = (windSpeedArray[2]-windSpeedArray[0])/2;
  DEBUG_println("stopWind index #" + String(windSpeedAverageIndex) +" -> " + String(windSpeedAverage[windSpeedAverageIndex]));
  if (++windSpeedAverageIndex >= 6 ) {
    windSpeedAverageIndex = 0;
    windSpeedNow = 0;
    for (byte i=0;i<6;i++ ) windSpeedNow = windSpeedNow + windSpeedAverage[i];
    windSpeed10Average[windSpeed10AverageIndex] = windSpeedNow / 6;
    if (++windSpeed10AverageIndex >= 10) windSpeed10AverageIndex = 0;
  }
  windSpeedNow = 0;
  for (byte i= 0;i<6;i++) windSpeedNow += windSpeedAverage[i];
  if (windSpeedNow > 0) {
      DEBUG_println("WindSpeed now is (km/h):" + String(windSpeedPerTic * (1000.0/ (float)(windSpeedNow/6))));
      return windSpeedPerTic * (1000.0/ (float)(windSpeedNow/6));
  } else return 0;
}


// get Averange of 10 Minutes
// WMO definition
float AOK5055::getWindSpeed(float windSpeedPerTic) {
   unsigned long ws = 0;
   for (byte i= 0;i<10;i++) ws += windSpeed10Average[i];
   ws = ws / 10;
   printf("WindSpeed : %ld ms\n",ws);
   printf("WindSpeed : %f 1/s\n",(1000.0/ (float)(ws)));
   printf("WindSpeed : %f km/h\n",windSpeedPerTic * (1000.0/ (float)(ws)));
   return windSpeedPerTic * (1000.0/ (float)(ws));
}


float AOK5055::getWindSpeedMax(float windSpeedPerTic) {
  // of  last 10 Minute (60 meassures)
  unsigned short ws = 0xffff;
  for (byte i= 0;i<10;i++) {
    if ((windSpeed10Average[i] < ws )) ws = windSpeed10Average[i];
  }
  return windSpeedPerTic * (1000.0/ (float)(ws));
}

boolean  AOK5055::getWindAlarm(byte windAlarmValue, float windSpeedPerTic) {
  return ( getWindSpeedNow(windSpeedPerTic) >= windAlarmValue);
}


byte AOK5055::getWindDirektion() {
  // one Serial steam 1 Start Bits 7 data
  // not realised by SoftwareSerial

  int i;
  unsigned long m;
  digitalWrite(this->powerPin, HIGH); // Power on
  // Search Start Bit
  m = pulseIn(windDirectionPin, HIGH,20000000);
  if  (m == 0) {
    DEBUG_println("getWindDirektion: TimeOut start bit not found");
    return 255;
  }
//   DEBUG_println("Found: " + String(m));
//  Format: 010dddd
  uint8_t direction = 0;
  for (i=6;i>=0; i--) {
    m = pulseIn(windDirectionPin,HIGH,20000);
//    DEBUG_println(m);
    if ( m >750 ) bitSet(direction,i);
  }
  digitalWrite(this->powerPin, LOW); // Power off
  direction = (direction & 0x0f);
  DEBUG_println("getWindDirektion :" + String(direction) );
  return direction;

  }

// Rain calculation


void AOK5055::countOneMinute(){
  rain1hArray[rain1hIndex] = rainTicks;
  if (++rain1hIndex >= 60) {
    rain1hIndex = 0;
    rain24hArray[rain24hIndex] = rainTicks;
    if (++rain24hIndex >= 24) rain24hIndex = 0;
  }
}

float AOK5055::getRain(float rainPerTic) {  // Return Rain of last cycle
  float rain = rainPerTic * (float)(rainTicks-rainCycle);
  DEBUG_print("getRain Tics #" + String(rainTicks));
  DEBUG_print(" getRain Tics #" + String(rainTicks-rainCycle));
  DEBUG_println("    Rain :" + String(rain));
  rainCycle = rainTicks;
return rain;
}

float AOK5055::getRain1h(float rainPerTic){
  byte firstIndex;
  if (rain1hIndex < 60 ) firstIndex = rain1hIndex + 1;
  else firstIndex = 0;
  return  rainPerTic * (float)(rain1hArray[rain1hIndex] - rain1hArray[firstIndex]);
}

float AOK5055::getRain24h(float rainPerTic){ // Return Rain of last 24h
  byte firstIndex;
  if (rain24hIndex < 24 ) firstIndex = rain1hIndex + 1;
  else firstIndex = 0;
  return  rainPerTic * (float)(rain24hArray[rain24hIndex] - rain24hArray[firstIndex]);
}
