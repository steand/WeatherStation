/*
RainSensor.cpp - This class implements the rain sensors.
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
#include "RainSensor.h"
#include "Debug.h"


#define ALARMRESET 10  // alarmLevel + ALARMRESET will reset alarm

// Rain sensor



RainSensor::RainSensor() {

}

RainSensor::RainSensor(uint8_t adcPin, uint8_t powerPin,unsigned int alarmLevel) {
  begin(adcPin, powerPin, alarmLevel);
}

void RainSensor::begin(uint8_t adcPin, uint8_t powerPin, unsigned int alarmLevel) {
  this->adcPin = adcPin;
  this->powerPin = powerPin;
  this->alarmLevel = alarmLevel;
  this->alarm = false;
  // pinMode(this->adcPin,INPUT);
  adcAttachPin(this->adcPin);
  pinMode(this->powerPin,OUTPUT);
}

unsigned int RainSensor::read(){
  digitalWrite(this->powerPin, HIGH);
  delay(100);
  unsigned int i=analogRead(this->adcPin);
  DEBUG_printf("Rain pin: %d, Value: %d\n",this->adcPin,i);
  digitalWrite(this->powerPin, LOW);
  if (i < this->alarmLevel) this->alarm = true;
  if (i > this->alarmLevel+ALARMRESET) this->alarm = false;
  return i;
}

boolean RainSensor::getAlarm() {
  DEBUG_printf("Rain alarm is %d\n",this->alarm);
  return this->alarm;

}

void RainSensor::setAlarmLevel(unsigned int alarmLevel) {
  this->alarmLevel = alarmLevel;
}
