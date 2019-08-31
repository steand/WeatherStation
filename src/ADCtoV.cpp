/*
ADC2V.cpp - implementation of read ADC voltage.
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
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "ADCtoV.h"

#if __has_include("Debug.h")
#include "Debug.h"
#endif

#define DEFAULT_VREF 1100




void print_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        DEBUG_printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        DEBUG_printf("Characterized using eFuse Vref\n");
    } else {
        DEBUG_printf("Characterized using Default Vref\n");
    }
}

ADCtoV::ADCtoV(adc_unit_t unit){
    setup(unit,ADC_WIDTH_BIT_12,ADC_ATTEN_DB_11);
}
ADCtoV::ADCtoV(adc_unit_t unit, adc_bits_width_t width){
  setup(unit,width,ADC_ATTEN_DB_11);
}
ADCtoV::ADCtoV(adc_unit_t unit, adc_atten_t atten){
  setup(unit,ADC_WIDTH_BIT_12,atten);
}
ADCtoV::ADCtoV(adc_unit_t unit, adc_bits_width_t width,adc_atten_t atten){
  setup(unit,width,atten);
}

void ADCtoV::setup(adc_unit_t unit, adc_bits_width_t width,adc_atten_t atten){
    this->width = width;
    this->atten = atten;
    if (unit == ADC_UNIT_1) adc1_config_width(this->width);
    this->adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
    #ifdef DEBUG
      print_val_type(val_type);
    #endif
}

void ADCtoV::attachChannel(adc1_channel_t channel){
   adc1_config_channel_atten(channel, this->atten);
}
void ADCtoV::attachChannel(adc2_channel_t channel){
   adc2_config_channel_atten(channel, this->atten);
}

float ADCtoV::read(adc1_channel_t channel){
  int reading =  adc1_get_raw(channel);
  return (float)esp_adc_cal_raw_to_voltage(reading, this->adc_chars)/1000.0;
}
float ADCtoV::read(adc1_channel_t channel,float v_divider){
  int reading =  adc1_get_raw(channel);
  return (float)esp_adc_cal_raw_to_voltage(reading, this->adc_chars)*v_divider/1000.0F;
};
float ADCtoV::read(adc2_channel_t channel){
  int *raw_out = NULL;
  adc2_get_raw(channel, this->width, raw_out);
  return (float)esp_adc_cal_raw_to_voltage((uint32_t)raw_out, this->adc_chars)/1000.0;
}
float ADCtoV::read(adc2_channel_t channel,float v_divider){
  int *raw_out = NULL;
  adc2_get_raw(channel, this->width, raw_out);
  return (float)esp_adc_cal_raw_to_voltage((uint32_t)raw_out, this->adc_chars)*v_divider/1000.0;
}
