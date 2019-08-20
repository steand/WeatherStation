/*
 * Debug.h
 *
 *  Created on: 01.02.2019
 *      Author: stefan
 */

#ifndef DEBUG_H_
#define DEBUG_H_


#define DEBUG //"Schalter" zum aktivieren

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
