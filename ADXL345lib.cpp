// Code by Blaise Jarrett @ OSEPP http://blaisejarrett.com
// Released to the public domain! Enjoy!

#include "ADXL345lib.h"
#include <stdio.h>
#include <Wire.h> 

#define WIRE Wire

#if (ARDUINO >= 100)
 #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
#else
 #include <WProgram.h>
#endif

Accelerometer::Accelerometer() : i2cAddr_(0), sensitivity_(ADXL345_RANGE_PM2G) {}

int8_t Accelerometer::begin(uint8_t i2cAddr)
{
    uint8_t devId;
    i2cAddr_ = i2cAddr;

    // Join the I2C bus as master
    WIRE.begin();

    if (i2cAddr_ == 0)
        return AD_ACC_ERROR_NOT_INITIALIZED;

    // Read the device ID just to make sure we are talking to the correct sensor 
    if (i2cReadBytes(ADXL345_REG_DEVID, &devId, 1) != 0) 
        return AD_ACC_ERROR_I2CREAD;

    if (devId != ADXL345_DEVID)
        return AD_ACC_ERROR_WRONG_DEV;

    // Set 25 Hz output data rate and 25 Hz bandwidth and disable low power mode 
    i2cWriteByte(ADXL345_REG_BWRATE, 0x08);
    // Disable auto sleep 
    i2cWriteByte(ADXL345_REG_PWCTL, 0x08);
    // Disable interrupts (the pins are not brought out anyway) 
    i2cWriteByte(ADXL345_REG_INTEN, 0x00);


    setSensitivity(ADXL345_RANGE_PM2G);

    return 0;
}

int8_t Accelerometer::readGs(double * x, double * y, double * z)
{
    int16_t r_x, r_y, r_z;
    int8_t raw_error;

    raw_error = readRaw(&r_x, &r_y, &r_z);
    // pass the error from readRaw if it failed
    if (raw_error != 0)
        return raw_error;

    // perform the float conversion
    int divisor;

    switch(sensitivity_)
    {
        case ADXL345_RANGE_PM2G:
            divisor = 256;
            break;
        case ADXL345_RANGE_PM4G:
            divisor = 128;
            break;
        case ADXL345_RANGE_PM8G:
            divisor = 64;
            break;
        case ADXL345_RANGE_PM16G:
            divisor = 32;
            break;

        default:
            // assume 4g?
            divisor = 128;
            break;
    }

    *x = (double)r_x / divisor;
    *y = (double)r_y / divisor;
    *z = (double)r_z / divisor;

    return 0;
}

int8_t Accelerometer::readRaw(int16_t * x, int16_t * y, int16_t * z)
{
    if (i2cAddr_ == 0)
        return AD_ACC_ERROR_NOT_INITIALIZED;

    uint8_t data[6];

    if (i2cReadBytes(ADXL345_REG_DATAX0, data, 6) != 0)
        return AD_ACC_ERROR_I2CREAD;

    conv2Byte2Signed16(data[0], data[1], x);
    conv2Byte2Signed16(data[2], data[3], y);
    conv2Byte2Signed16(data[4], data[5], z);

    return 0;
}

void Accelerometer::conv2Byte2Signed16(uint8_t LSB, uint8_t MSB, int16_t * dest)
{
    *dest = 0;

    *dest = (int16_t)LSB;

    *dest |= ((int16_t)MSB << 8);
}

int8_t Accelerometer::setOffsets(int8_t x, int8_t y, int8_t z)
{
    int8_t writeByteError;

    writeByteError = i2cWriteByte(ADXL345_REG_OFSX, x);
    if (writeByteError != 0)
        return writeByteError;

    writeByteError = i2cWriteByte(ADXL345_REG_OFSY, y);
    if (writeByteError != 0)
        return writeByteError;

    writeByteError = i2cWriteByte(ADXL345_REG_OFSZ, z);
    if (writeByteError != 0)
        return writeByteError;
}

int8_t Accelerometer::i2cReadBytes(uint8_t reg, uint8_t *data, uint8_t len)
{
    if (i2cAddr_ == 0)
        return AD_ACC_ERROR_NOT_INITIALIZED;

    WIRE.beginTransmission(i2cAddr_);
    // request a read from this address
    WIRE.write(reg);
    // end to allow the response
    WIRE.endTransmission();

    // Read a byte from the device
    WIRE.requestFrom(i2cAddr_, len); 

    uint8_t i = 0;

    while (WIRE.available())
    {
        data[i] = WIRE.read();
        ++i;

        if (i == len)
            break;
    }

    if (i != len)
        return -1;

    return 0;
}

int8_t Accelerometer::i2cWriteByte(uint8_t reg, uint8_t data)
{
    if (i2cAddr_ == 0)
        return AD_ACC_ERROR_NOT_INITIALIZED;

    // Begin the write sequence 
    WIRE.beginTransmission(i2cAddr_);

    // First byte is to set the register pointer
    WIRE.write(reg);

    // Write the data byte
    WIRE.write(data);

    // End the write sequence; bytes are actually transmitted now 
    WIRE.endTransmission();

    return 0;
}

int8_t Accelerometer::setSensitivity(uint8_t sensitivity)
{
    uint8_t tmp = 0;

    if (sensitivity > 3)
        return AD_ACC_ERROR_SENS_OUTOFRANGE;

    sensitivity_ = sensitivity;

    tmp = sensitivity & 0x03;

    if (i2cWriteByte(ADXL345_REG_DATAFORMAT, tmp) != 0)
        return AD_ACC_ERROR_I2CWRITE;

    return 0;
}