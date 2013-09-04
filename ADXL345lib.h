// Code by Blaise Jarrett @ OSEPP http://blaisejarrett.com
// Released to the public domain! Enjoy!

#ifndef _ADXL345LIB_H_
#define _ADXL345LIB_H_

#include <stdint.h>

#define OSEPP_ACC_SW_ON 0x53
#define OSEPP_ACC_SW_OFF 0x1D
#define ADXL345_DEVID 0xE5

#define AD_ACC_ERROR_NOT_INITIALIZED -2
#define AD_ACC_ERROR_WRONG_DEV -3
#define AD_ACC_ERROR_I2CREAD -4
#define AD_ACC_ERROR_I2CWRITE -5
#define AD_ACC_ERROR_SENS_OUTOFRANGE -6

// Register Map
#define ADXL345_REG_DEVID 0x00
#define ADXL345_REG_BWRATE 0x2C
#define ADXL345_REG_PWCTL 0x2D
#define ADXL345_REG_INTEN 0x2E
#define ADXL345_REG_DATAFORMAT 0x31
#define ADXL345_REG_DATAX0 0x32
#define ADXL345_REG_DATAX1 0x33
#define ADXL345_REG_DATAY0 0x34
#define ADXL345_REG_DATAY1 0x35
#define ADXL345_REG_DATAZ0 0x36
#define ADXL345_REG_DATAZ1 0x37

#define ADXL345_RANGE_PM2G 0
#define ADXL345_RANGE_PM4G 1
#define ADXL345_RANGE_PM8G 2
#define ADXL345_RANGE_PM16G 3


struct ThreeAxisF_t {
    double x;
    double y;
    double z;
};

struct ThreeAxisI_t {
    int16_t x;
    int16_t y;
    int16_t z;
};

class Accelerometer {
    public:
        Accelerometer();
        int8_t begin(uint8_t i2cAddr);
        int8_t setSensitivity(uint8_t sensitivity);
        ThreeAxisI_t readRaw();
        ThreeAxisF_t readGs();
    protected:
        int8_t i2cReadBytes(uint8_t reg, uint8_t * data, uint8_t len);
        int8_t i2cWriteByte(uint8_t reg, uint8_t data);
        uint8_t i2cAddr_;
        uint8_t sensitivity_;
    private:
        void conv2Byte2Signed16(uint8_t LSB, uint8_t MSB, int16_t * dest);
};


#endif