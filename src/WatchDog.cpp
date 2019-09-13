/*
WatchDog.cpp - implement watchDog
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
#include "WatchDog.h"
#include "esp_system.h"
#include <rom/rtc.h>

static RTC_NOINIT_ATTR  short resetCount;

void IRAM_ATTR watchDog(){
  ESP.restart();
}

WatchDog::WatchDog(){};

void WatchDog::start(byte time) {
  timer = timerBegin(0, 80, true);         //timer 0, div 80
  timerAttachInterrupt(timer, &watchDog, true);
  timerAlarmWrite(timer, time*1000000, false); //set time in us
  timerAlarmEnable(timer);                //enable interrupt
}

unsigned short WatchDog::isSoftwareReset() {
  if (rtc_get_reset_reason(0) == SW_CPU_RESET) {
    resetCount++;
  } else {
    resetCount=0;
  }
  return resetCount;
}

void WatchDog::feed() {
  timerWrite(timer, 0);
}
