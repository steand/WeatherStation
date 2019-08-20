/*
 * WeatherMQTT.h
 *
 *  Created on: 06.02.2019
 *      Author: stefan
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
