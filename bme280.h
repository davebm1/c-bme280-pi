#ifndef __BME280_H
#define __BME280_H

/* edit device DEV_ID, DEV_PATH and LOCAL_HASL as needed */
#define DEV_ID 0x76
#define DEV_PATH "/dev/i2c-1"
#define LOCAL_HASL 25.0 /* local height above sea level  m */

#define IDENT 0xD0
#define SOFT_RESET 0xE0
#define CTRL_HUM 0xF2
#define STATUS 0xF3
#define CTRL_MEAS 0xF4
#define CONFIG 0xF5

#define DATA_START_ADDR 0xF7
#define DATA_LENGTH 8

/* sea level pressure conversion constants */
#define G 9.80665   /* acceleration due to gravity  m/s2 */
#define M 0.0289644 /* molar mass of Earth's air  kg/mol */
#define T 288.15    /* standard temperature  K */
#define R 8.3144598 /* universal gas constant  J/(mol·K) */

double sta2sea(double station_press);

#endif
