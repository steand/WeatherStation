/*
AOK5055.h - Include file for class "AOK5055".
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

#ifndef AOK5055_H_
#define AOK5055_H_

#include <Arduino.h>

class AOK5055 {

private:
uint8_t rainPin;
uint8_t powerPin;
uint8_t windSpeedPin;
uint8_t windDirectionPin;



public:
  AOK5055();
  void     begin(uint8_t rainPin,uint8_t powerPin,
                 uint8_t windSpeedPin, uint8_t windDirectionPin);
  void     startWindSpeed();
  float    stopWindSpeed(float windSpeedPerTic);
  float    getWindSpeed(float windSpeedPerTic);       // get mean of 6 last meassures (~1 minute)
  float    getWindSpeedNow(float windSpeedPerTic);       // get mean of 60 last meassures (~1 minute)
  void     countOneMinute();                          // must call every minute
  float    getWindSpeedMax(float windSpeedPerTic);    // get max of 6 last meassures (~1 minute)
  boolean  getWindAlarm(byte windAlarmValue, float windSpeedPerTic);
  byte     getWindDirektion();
  float    getRain(float rainPerTic);     // mm of last cycle
  float    getRain1h(float rainPerTic);  // mm of last 60 meassures accuracy by 5 (~1 hour) (most use)
  float    getRain24h(float rainPerTic);  // mm of last 1440 meassures accuracy by 60 (~24 hour)

};

#endif
