/**
 * @file
 * @author  Blaise Jarrett <me@blaisejarrett.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Released to the public domain! Enjoy!
 *
 * @section DESCRIPTION
 *
 * The Accelerometer header for the ADXL345 Accelerometer class.
 */

#ifndef _ADXL345LIB_H_
#define _ADXL345LIB_H_

#include <stdint.h>

/** 
 * The I2C address of the OSEPP Accelerometer sensor
 * when the physical switch is in the ON position.
 */
#define OSEPP_ACC_SW_ON 0x53
/** 
* The I2C address of the OSEPP Accelerometer sensor
* when the physical switch is in the OFF position.
*/
#define OSEPP_ACC_SW_OFF 0x1D
#define ADXL345_DEVID 0xE5

/** Forgot to call begin() */
#define AD_ACC_ERROR_NOT_INITIALIZED -2
/** Device did not respond with the correct ID */
#define AD_ACC_ERROR_WRONG_DEV -3
/** I2C read error */
#define AD_ACC_ERROR_I2CREAD -4
/** I2C write error */
#define AD_ACC_ERROR_I2CWRITE -5
/** Range argument must be one of the definitions */
#define AD_ACC_ERROR_SENS_OUTOFRANGE -6

// Register Map
#define ADXL345_REG_DEVID 0x00
#define ADXL345_REG_OFSX 0x1E
#define ADXL345_REG_OFSY 0x1F
#define ADXL345_REG_OFSZ 0x20
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

/** Range: +-2G */
#define ADXL345_RANGE_PM2G 0
/** Range: +-4G */
#define ADXL345_RANGE_PM4G 1
/** Range: +-8G */
#define ADXL345_RANGE_PM8G 2
/** Range: +-16G */
#define ADXL345_RANGE_PM16G 3

/**
 * Represents a ADXL345 bassed Accelerometer sensor.
 * You must call begin before calling any other methods.
 */
class Accelerometer {
    public:
        Accelerometer();

        /**
         * Attenpts to connect to the device. You must call begin before calling any other method.
         *
         * @param i2cAddr Takes the I2C address of the Gyroscope as an argument. 
         *                Use #OSEPP_ACC_SW_ON if your address switch is in the ON position. 
         *                Use #OSEPP_ACC_SW_OFF if your address switch is in the OFF position.
         *
         * @return Returns 0 if it successfully connects to the Gyroscope sensor. 
         *         Returns any other non-zero number if it fails.
         *         Check ADXL345lib.h for the various ERROR definitions.
         */
        int8_t begin(uint8_t i2cAddr);

        /**
         * The ADXL345 has an adjustable range to help you fine tune the sensitivity.
         *
         * @param sensitivity The range definition. Check ADXL345lib.h for possible arugments.
         *                    Defaults range is +-2G.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check ADXL345lib.h for the various ERROR definitions.
         */
        int8_t setSensitivity(uint8_t sensitivity);

        /**
         * Reads the RAW data from the accelerometer as a signed 16 bit integer.
         * This integer does not represent Gs. You'll probably 
         * want to use readGs instead.
         *
         * @param x A pointer to the variable that will get the x raw data recorded to.
         * @param y A pointer to the variable that will get the y raw data recorded to.
         * @param z A pointer to the variable that will get the z raw data recorded to.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check ADXL345lib.h for the various ERROR definitions.
         */
        int8_t readRaw(int16_t * x, int16_t * y, int16_t * z);

        /**
         * Reads the Gs experienced by the Accelerometer.
         *
         * @param x A pointer to the variable that will get the x data recorded to (in Gs).
         * @param y A pointer to the variable that will get the y data recorded to (in Gs).
         * @param z A pointer to the variable that will get the z data recorded to (in Gs).
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check ADXL345lib.h for the various ERROR definitions.
         */
        int8_t readGs(double * x, double * y, double * z);

        /**
         * Sets the calibration offset registers on the ADXL345. These
         * offsets are subtracted from the raw data values. A range of 15.6mg/LSB is used.
         * Refer to the calibration example.
         *
         * @param x The x offset value to be set in the ADXL345 registers.
         * @param y The y offset value to be set in the ADXL345 registers.
         * @param z The z offset value to be set in the ADXL345 registers.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check ADXL345lib.h for the various ERROR definitions.
         */
        int8_t setOffsets(int8_t x, int8_t y, int8_t z);
    protected:
        /**
         * I2C interface function for reading bytes. Uses the Arduino Wire library.
         *
         * @param reg The first register to read from.
         * @param data The array where the data will be read into.
         * @param len The number of bytes to read.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check ADXL345lib.h for the various ERROR definitions.
         */
        int8_t i2cReadBytes(uint8_t reg, uint8_t * data, uint8_t len);

        /**
         * I2C interface function for writing a single byte. Uses the Arduino Wire library.
         *
         * @param reg The register address to write to.
         * @param data The byte to write.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check ADXL345lib.h for the various ERROR definitions.
         */
        int8_t i2cWriteByte(uint8_t reg, uint8_t data);

        /** The I2C address of the device. */
        uint8_t i2cAddr_;

        /** The range definition enum of the device. */
        uint8_t sensitivity_;
    private:
        /**
         * Combines two bytes into a 16 bit 2s compliment number.
         *
         * @param LSB The LSB 2s compliment data.
         * @param MSB The MSB 2s compliment data.
         * @param dest The signed 16 bit number result.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check ADXL345lib.h for the various ERROR definitions.
         */
        void conv2Byte2Signed16(uint8_t LSB, uint8_t MSB, int16_t * dest);
};


#endif