#include <Wire.h>
#include "ADXL345lib.h"


Accelerometer acc;
bool fail;

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
        fail = true;
        return;
    }

    acc.setSensitivity(ADXL345_RANGE_PM4G);
}

void loop()
{
    // don't bother reading if we failed to connect
    if (fail)
        return;

    // 3 doubles to store the data
    double x, y, z;

    // Read Gs
    // Note: You can also read the raw data by calling
    //       readRaw() - readRaw takes int16_t instead of doubles
    if (acc.readGs(&x, &y, &z) != 0)
    {
        Serial.println("Failed to read accelerometer");
        return;
    }
    
    // print them out
    Serial.print("X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.print(" Z: ");
    Serial.println(z);

    delay(50);
}
