/*
WatchDog.h - Include file.
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

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "Arduino.h"
#include "esp_system.h"
#include <rom/rtc.h>

class WatchDog{
private:
   hw_timer_t *timer = NULL;

public:
   WatchDog();
   void start(byte time); // time in second
   void feed();
   unsigned short isSoftwareReset();
};

#endif /* DEBUG_H_ */
