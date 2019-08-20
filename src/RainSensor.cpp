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
  DEBUG_printf("Rain pin is %d\n",this->adcPin);
  DEBUG_printf("Rain is %d\n",i);
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
