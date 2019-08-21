/*
RainSensor.h - Include file for class "RainSensor".
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

#ifndef RAINSENSOR_H_
#define RAINSENSOR_H_

#include "Arduino.h"



class RainSensor{

private:
  uint8_t adcPin;
  uint8_t powerPin;
  unsigned int alarmLevel;
  boolean alarm;

public:

RainSensor();
RainSensor(uint8_t adcPin, uint8_t powerPin,unsigned int alarmLevel);

void begin(uint8_t adcPin, uint8_t powerPin,unsigned int alarmLevel);
void setAlarmLevel(unsigned int alarmLevel);
unsigned int read();
boolean getAlarm();


};

#endif /* SENSORS_H_ */
