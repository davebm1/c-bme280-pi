#include "compensation.h"

#include <i2c/smbus.h>

static int32_t t_fine = 0;

static uint16_t dig_T1 = 0;
static int16_t dig_T2 = 0;
static int16_t dig_T3 = 0;

static uint16_t dig_P1 = 0;
static int16_t dig_P2 = 0;
static int16_t dig_P3 = 0;
static int16_t dig_P4 = 0;
static int16_t dig_P5 = 0;
static int16_t dig_P6 = 0;
static int16_t dig_P7 = 0;
static int16_t dig_P8 = 0;
static int16_t dig_P9 = 0;

static uint8_t dig_H1 = 0;
static int16_t dig_H2 = 0;
static uint8_t dig_H3 = 0;
static int16_t dig_H4 = 0;
static int16_t dig_H5 = 0;
static int8_t dig_H6 = 0;

/* The following three compensation functions are reproduced from Appendix A: Section 8.1,
 * Compensation formulas in double precision floating point, of the Bosch Sensortec technical
 * datasheet.
 *
 * https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
 *
 */

/* Returns temperature in DegC, double precision. Output value of “51.23”
 * equals 51.23 DegC. t_fine carries fine temperature as global value
 */
double BME280_compensate_T_double(int32_t adc_T) {
    double var1, var2, T;
    var1 = (((double)adc_T) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
    var2 = ((((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0) *
            (((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0)) *
           ((double)dig_T3);
    t_fine = (int32_t)(var1 + var2);
    T = (var1 + var2) / 5120.0;
    return T;
}

/* Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa
 * = 963.862 hPa
 */
double BME280_compensate_P_double(int32_t adc_P) {
    double var1, var2, p;
    var1 = ((double)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
    var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);
    /* avoid exception caused by division by zero */
    if (var1 == 0.0) {
        return 0;
    }
    p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
    return p;
}

/* Returns humidity in %rH as as double. Output value of “46.332”
 * represents 46.332 %rH
 */
double BME280_compensate_H_double(int32_t adc_H) {
    double var_H;
    var_H = (((double)t_fine) - 76800.0);
    var_H = (adc_H - (((double)dig_H4) * 64.0 + ((double)dig_H5) / 16384.0 * var_H)) *
            (((double)dig_H2) / 65536.0 *
             (1.0 + ((double)dig_H6) / 67108864.0 * var_H *
                        (1.0 + ((double)dig_H3) / 67108864.0 * var_H)));
    var_H = var_H * (1.0 - ((double)dig_H1) * var_H / 524288.0);
    if (var_H > 100.0)
        var_H = 100.0;
    else if (var_H < 0.0)
        var_H = 0.0;
    return var_H;
}

/* Read calibration data and determine trimming parameters */
void setCompensationParams(int fd) {
    uint8_t calData0[25];
    uint8_t calData1[7];

    /* read calibration data */
    i2c_smbus_read_i2c_block_data(fd, CAL_DATA0_START_ADDR, CAL_DATA0_LENGTH, calData0);
    i2c_smbus_read_i2c_block_data(fd, CAL_DATA1_START_ADDR, CAL_DATA1_LENGTH, calData1);

    /* trimming parameters */
    dig_T1 = calData0[1] << 8 | calData0[0];
    dig_T2 = calData0[3] << 8 | calData0[2];
    dig_T3 = calData0[5] << 8 | calData0[4];

    dig_P1 = calData0[7] << 8 | calData0[6];
    dig_P2 = calData0[9] << 8 | calData0[8];
    dig_P3 = calData0[11] << 8 | calData0[10];
    dig_P4 = calData0[13] << 8 | calData0[12];
    dig_P5 = calData0[15] << 8 | calData0[14];
    dig_P6 = calData0[17] << 8 | calData0[16];
    dig_P7 = calData0[19] << 8 | calData0[18];
    dig_P8 = calData0[21] << 8 | calData0[20];
    dig_P9 = calData0[23] << 8 | calData0[22];

    dig_H1 = calData0[24];
    dig_H2 = calData1[1] << 8 | calData1[0];
    dig_H3 = calData1[2];
    dig_H4 = calData1[3] << 4 | (calData1[4] & 0xF);
    dig_H5 = calData1[5] << 4 | (calData1[4] >> 4);
    dig_H6 = calData1[6];
}
