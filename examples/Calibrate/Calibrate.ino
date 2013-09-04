/*
 * Run this program and open the serial monitor.
 * Follow the instructions printed over serial.
 */

#include <Wire.h>
#include "ADXL345lib.h"
#include <math.h>

#define SAMPLESIZE 15

Accelerometer acc;

int8_t gatherAverage(int16_t * average_x, int16_t * average_y, int16_t * average_z)
{
    Serial.println("Gathering Average...");

    int32_t accum_x = 0, accum_y = 0, accum_z = 0;

    for (uint8_t i = 0; i < SAMPLESIZE; ++i)
    {
        int16_t x, y, z;

        if (acc.readRaw(&x, &y, &z) != 0)
        {
            Serial.println("Failed to read sensor. End.");
            return -1;
        }

        accum_x += x;
        accum_y += y;
        accum_z += z;

        // sensor runs at 25hz by default
        // wait atleast 1/25th of a second before attempting
        // another read or we will read the same value
        delay(50);
    }

    // average values
    *average_x = accum_x / SAMPLESIZE;
    *average_y = accum_y / SAMPLESIZE;
    *average_z = accum_z / SAMPLESIZE;


    Serial.println("Averages Gathered:");
    Serial.print("X: ");
    Serial.print(*average_x);
    Serial.print(" Y: ");
    Serial.print(*average_y);
    Serial.print(" Z: ");
    Serial.println(*average_z);

    return 0;
}

void setup()
{
    Serial.begin(57600);
    
    // set the I2C address of the accelerometer
    //
    // With the OSEPP Accelerometer use:
    //      OSEPP_ACC_SW_ON    - Switch is in the ON position
    //      OSEPP_ACC_SW_OFF   - Switch is in the OFF position
    //
    // begin will return 0 on success
    if (acc.begin(OSEPP_ACC_SW_ON) != 0)
    {
        Serial.println("Error connecting to accelerometer");
        return;
    }

    // use the most sensitive mode for calibration
    // at this sensitivity we have 256 values / g
    acc.setSensitivity(ADXL345_RANGE_PM2G);
    // clear any existing offsets
    acc.setOffsets(0, 0, 0);


    Serial.println("This example will calculate offset values");
    Serial.println("to calibrate the accelerometer. Record");
    Serial.println("these values and enter them in your own");
    Serial.println("projects using this library.");
    Serial.println();

    Serial.println("Firmy hold the accelerometer upside down on a flat");
    Serial.println("surface and type \"k\" when ready.");
    Serial.println();


    while (true)
    {
        if (Serial.available())
        {
            char inByte = Serial.read();
            
            if (inByte == 'k' || inByte == 'K')
                break;
        }
    }

    int16_t average_x, average_y, average_z;

    if (gatherAverage(&average_x, &average_y, &average_z) != 0)
        return;

    Serial.println();
    Serial.println("Cacluating Offsets...");

    int8_t offset_x, offset_y, offset_z;

    // we have to divide by four because the offsent sensitivity
    // is 15.6mg vs 3.9mg of the 2g sensitivity
    offset_x = 0 - round((double)average_x / 4);
    offset_y = 0 - round((double)average_y / 4);
    offset_z = 0 - round((double)(average_z + 256) / 4);

    Serial.println("Save these values for use in your program:");
    Serial.print("X: ");
    Serial.print(offset_x);
    Serial.print(" Y: ");
    Serial.print(offset_y);
    Serial.print(" Z: ");
    Serial.println(offset_z);


    Serial.println();
    Serial.print("Setting Offsets with \"acc.setOffsets(");
    Serial.print(offset_x);
    Serial.print(", ");
    Serial.print(offset_y);
    Serial.print(", ");
    Serial.print(offset_z);
    Serial.print(");\"");
    Serial.println();
    Serial.println();

    acc.setOffsets(offset_x, offset_y, offset_z);

    if (gatherAverage(&average_x, &average_y, &average_z) != 0)
        return;
}

void loop()
{

}
