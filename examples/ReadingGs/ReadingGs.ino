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

    // a struct of 3 doubles: x, y, z
    ThreeAxisF_t xyz;

    // Read Gs
    // Note: You can also read the raw data by calling
    //       readRaw() - This will return a ThreeAxisI_t
    //                   a struct of 3 int16s: x, y, z
    xyz = acc.readGs();
    
    // print them out
    Serial.print("X: ");
    Serial.print(xyz.x);
    Serial.print(" Y: ");
    Serial.print(xyz.y);
    Serial.print(" Z: ");
    Serial.println(xyz.z);

    delay(50);
}
