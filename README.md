# Bosch Sensortec BME280 Combined humidity and pressure sensor

## C language demonstration code for the Raspberry Pi

### Tested with: Raspberry Pi 4B / Raspbian GNU/Linux 11 (bullseye)

Prerequisites:

1. sudo raspi-config --> interfacing options --> enable i2c
2. sudo apt install libi2c-dev i2c-tools
3. determine i2c busses with i2cdetect -l
4. determine BME280 device location and id with i2cdetect -y \<bus number\>
5. edit DEV_ID / DEV_PATH in bme280.h if necessary
6. edit LOCAL_HASL for local height above sea level, if required

Build / run

1. make
2. ./bme280
3. ctrl-C to terminate

C code for the Raspberry Pi with the BME280 sensor, using direct calls to
functions provided by libi2c-dev.

Readings are taken at a rate of 1 per second, in forced mode,
oversampling x 1 and filter off. Data can be streamed with a
measurement time down to 10 ms, with these settings, if needed.

Forced mode allows the sensor to sleep between readings.
