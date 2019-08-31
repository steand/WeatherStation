/*
ADC2V.h - Include file for read ADC voltage.
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

#ifndef ADCtoV_H_
#define ADCtoV_H_

#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "Debug.h"

class ADCtoV {

private:
  float v_divider;
  adc_bits_width_t width;
  adc_atten_t atten;
  esp_adc_cal_characteristics_t *adc_chars;
  void setup(adc_unit_t unit, adc_bits_width_t width,adc_atten_t atten);

public:
  ADCtoV(adc_unit_t unit);
  ADCtoV(adc_unit_t unit, adc_bits_width_t width);
  ADCtoV(adc_unit_t unit, adc_atten_t atten);
  ADCtoV(adc_unit_t unit, adc_bits_width_t width,adc_atten_t atten);
  void attachChannel(adc1_channel_t channel);
  void attachChannel(adc2_channel_t channel);
  /*
  Vin = Vadc*(R1+R2)/R1
  v_divider = (R1+R2)/R1
  Best is to messure the factor:
  v_divider = Vin/Vadc
  eq: Vin = 10V Vadc=3V  v_divider = 10/3 = 3.3333
  */
  float read(adc1_channel_t channel);
  float read(adc1_channel_t channel,float v_divider);
  float read(adc2_channel_t channel);
  float read(adc2_channel_t channel,float v_divider);
};

#endif /* ADC2V_H_ */
