/*
Debug.h - Include file for debuging via Serial.
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

#ifndef DEBUG_H_
#define DEBUG_H_

// for debuging on uncomment follow line else comment for debuging off
#define DEBUG

#ifdef DEBUG
#define DEBUG_print(__debug_x) Serial.print(__debug_x)
#define DEBUG_println(__debug_x) Serial.println(__debug_x)
#define DEBUG_printf(__debug_x,__debug_value) printf(__debug_x,__debug_value)
#define DEBUG_begin(__debug_x) Serial.begin(__debug_x)
#else
#define DEBUG_print(__debug_x)
#define DEBUG_println(__debug_x)
#define DEBUG_printf(__debug_x,__debug_value)
#define DEBUG_begin(__debug_x)
#endif


#endif /* DEBUG_H_ */
