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

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


// Rain handling
volatile unsigned long rainTicks;
volatile unsigned long lastRainTick;
unsigned long  rainCycle;           // Rain in the cycle
unsigned long  rainMinute;           // Rain in the cycle
unsigned short rain1hArray[60];     // rain last 60 minutes
byte           rain1hIndex;
unsigned short rain24hArray[24];    // rain last day
byte           rain24hIndex;


void IRAM_ATTR isrRain() {
  if ((millis()-lastRainTick) > 50) { // kill all bounds les 50 ms
     portENTER_CRITICAL_ISR(&mux);
    rainTicks++;
     portEXIT_CRITICAL_ISR(&mux);
    lastRainTick = millis();
  }
}

// Wind handling

volatile byte windTicks;
volatile unsigned long lastWindTick;
unsigned long startWindTime;
volatile unsigned long  windSpeedArray[3];  // max Windspeed messure points
unsigned short windSpeedAverage[6];  // 1 Minute (6*10) times every Minute
unsigned short windSpeed10Average[10]; // 10 Minute  times every Minute
byte     windSpeedAverageIndex;
byte     windSpeed10AverageIndex;

unsigned long messMillis;

void IRAM_ATTR isrWind() {
   messMillis = millis();
  if ((messMillis-lastWindTick) > 5) { // kill all bounds les 5 ms
    portENTER_CRITICAL_ISR(&mux);
    if (windTicks < 3) windSpeedArray[windTicks] = messMillis;
      windTicks++;
      lastWindTick = messMillis;
    portEXIT_CRITICAL_ISR(&mux);
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
  rainMinute = 0;
  for (i=0;i<60;i++) rain1hArray[i] = 0;     // tics last 60 minutes
  rain1hIndex=0;
  for (i=0;i<24;i++) rain24hArray[i]=0;    // tics last day
  rain24hIndex=0;
  for (i=0;i<6;i++) windSpeedAverage[i]=0xFFFF;
  for (i=0;i<10;i++) windSpeed10Average[i]=0xFFFF;
  windSpeedAverageIndex = 0;
  windSpeed10AverageIndex = 0;
  pinMode(rainPin,INPUT);
  pinMode(windSpeedPin,INPUT);
  pinMode(windDirectionPin,INPUT_PULLDOWN);
  pinMode(powerPin,OUTPUT);
  digitalWrite(this->powerPin, LOW); // Power off

   // Wind Trigger Pin Test only
  //  pinMode(4,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(this->rainPin),isrRain, FALLING);

}


unsigned long windStartTime;

void AOK5055::startWindSpeed() {
  windTicks=0;
  lastRainTick = millis();
  startWindTime=millis();
  attachInterrupt(digitalPinToInterrupt(this->windSpeedPin),isrWind, FALLING);
}

float AOK5055::stopWindSpeed(float windSpeedPerTic) {
  float windSpeedNow = 0.0;
  portENTER_CRITICAL(&mux);
  byte wt = windTicks;
  portEXIT_CRITICAL(&mux);
  while (((millis() - startWindTime) <= WIND_SPEED_MIN_MESSURE_TIME) &&
           (wt < 3) ) {
             delay(10);
             portENTER_CRITICAL(&mux);
             wt = windTicks;
             portEXIT_CRITICAL(&mux);
           }
  detachInterrupt(digitalPinToInterrupt(this->windSpeedPin));
  DEBUG_printf("WindSpeed needs: %lu ms",millis() - startWindTime);
  // ToDo Speed berechnen
  windSpeedAverage[windSpeedAverageIndex] = 0xFFFF; // time = 1/speed
  if (windTicks == 1 ) windSpeedAverage[windSpeedAverageIndex] = WIND_SPEED_MIN_MESSURE_TIME;
  if (windTicks == 2 ) windSpeedAverage[windSpeedAverageIndex] = windSpeedArray[1]-windSpeedArray[0];
  if (windTicks == 3 ) windSpeedAverage[windSpeedAverageIndex] = (windSpeedArray[2]-windSpeedArray[0])/2;
  if (windSpeedAverage[windSpeedAverageIndex] == 0) windSpeedAverage[windSpeedAverageIndex] = 0xFFFF; //secure
  DEBUG_printf("stopWind Index: #%d -> #%d\n", windSpeedAverageIndex,windSpeedAverage[windSpeedAverageIndex]);
  if (++windSpeedAverageIndex >= 6 ) {
    windSpeedAverageIndex = 0;
    windSpeedNow = 0;
    for (byte i=0;i<6;i++ ) windSpeedNow = windSpeedNow + 1.0F/windSpeedAverage[i];
    windSpeed10Average[windSpeed10AverageIndex] = 1/(windSpeedNow / 6);
    if (++windSpeed10AverageIndex >= 10) windSpeed10AverageIndex = 0;
  }
  windSpeedNow = 0;
  for (byte i= 0;i<6;i++) windSpeedNow += 1000.0F/windSpeedAverage[i];
  DEBUG_printf("WindSpeed now is %.1f km/h\n",windSpeedPerTic * windSpeedNow/6);
  return windSpeedPerTic * (windSpeedNow/6);
}


// get Averange of 10 Minutes
// WMO definition
float AOK5055::getWindSpeed(float windSpeedPerTic) {
   float ws = 0;
   for (byte i= 0;i<10;i++) ws += 1000.0F/windSpeed10Average[i];
   ws = ws / 10.0;
   DEBUG_printf("%f 1/s - %.1f km/h\n",ws, windSpeedPerTic * ws);
   return windSpeedPerTic * ws;
}


float AOK5055::getWindSpeedMax(float windSpeedPerTic) {
  // of  last 10 Minute (60 meassures)
  unsigned short ws = 0xffff;
  for (byte i= 0;i<10;i++) {
    if ((windSpeed10Average[i] < ws )) ws = windSpeed10Average[i];
  }
  return windSpeedPerTic * (1000.0F/ (float)(ws));
}


byte AOK5055::getWindDirektion() {
  // one Serial steam 1 Start Bits 7 data
  // not realised by SoftwareSerial

  int i;
  unsigned long m;
  digitalWrite(this->powerPin, HIGH); // Power on
  // Search Start Bit
  m = pulseIn(windDirectionPin, HIGH,1000000);
  if  (m == 0) {
    DEBUG_println("getWindDirektion: TimeOut start bit not found");
    return 255;
  }
   DEBUG_println("Found: " + String(m));
//  Format: 010dddd
  uint8_t direction = 0;
  for (i=6;i>=0; i--) {
    m = pulseIn(windDirectionPin,HIGH,5000);
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
  portENTER_CRITICAL(&mux);
  unsigned long rt=rainTicks;
  portEXIT_CRITICAL(&mux);
  unsigned short rain = rt-rainMinute;
  rain1hArray[rain1hIndex] = rain;
  rain24hArray[rain24hIndex] += rain;
  rainMinute = rt;
  DEBUG_printf("Last Minute rain #%d Ticks\n", rain1hArray[rain1hIndex] );
  if (++rain1hIndex >= 60) {
    rain1hIndex = 0;
    if (++rain24hIndex >= 24) rain24hIndex = 0;
    rain24hArray[rain24hIndex] = 0;
  }
}

float AOK5055::getRain(float rainPerTic) {  // Return Rain of last cycle
  portENTER_CRITICAL(&mux);
  unsigned long rt=rainTicks;
  portEXIT_CRITICAL(&mux);
  float rain = rainPerTic * (float)(rt-rainCycle);
  DEBUG_printf("getRain Tics Summe: #%lu, Delta: #%lu, Rain %fmm\n",rt,rt-rainCycle,rain);
  rainCycle = rt;
return rain;
}

float AOK5055::getRain1h(float rainPerTic){
  unsigned short rain=0;
  for (int i=0; i<60; i++) rain += rain1hArray[i];
  DEBUG_printf("Last Hour rain #%d Ticks = %f mm\n", rain, rainPerTic * (float)(rain));
  return  rainPerTic * (float)(rain);
}

float AOK5055::getRain24h(float rainPerTic){ // Return Rain of last 24h witgout last hour
  unsigned short rain=0;
  for (int i=0; i<24; i++) rain += rain24hArray[i];
  DEBUG_printf("Last 24Hour rain #%d Ticks\n", rain);
  return  rainPerTic * (float)(rain);
}
