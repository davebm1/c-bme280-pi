#ifndef __COMPENSATION_H
#define __COMPENSATION_H

#include <stdint.h>

#define CAL_DATA0_START_ADDR 0x88
#define CAL_DATA0_LENGTH 25
#define CAL_DATA1_START_ADDR 0xE1
#define CAL_DATA1_LENGTH 7

double BME280_compensate_T_double(int32_t adc_T);
double BME280_compensate_P_double(int32_t adc_P);
double BME280_compensate_H_double(int32_t adc_H);
void setCompensationParams(int fd);

#endif
