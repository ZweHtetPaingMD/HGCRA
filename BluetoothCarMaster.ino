#include "Wire.h"

const int flexPin = A0;
const int MPU_ADDR = 0x68;
int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t smo_accel_y, smo_accel_x;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;


void gyro();
void transmit();

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

void loop()
{
    gyro();
    transmit();
}

void gyro()
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 7 * 2, true);

    accelerometer_x = Wire.read() << 8 | Wire.read();
    accelerometer_y = Wire.read() << 8 | Wire.read();
    accelerometer_z = Wire.read() << 8 | Wire.read();
    temperature = Wire.read() << 8 | Wire.read();
    gyro_x = Wire.read() << 8 | Wire.read();
    gyro_y = Wire.read() << 8 | Wire.read();
    gyro_z = Wire.read() << 8 | Wire.read();
    smo_accel_y = accelerometer_y / 100 - 5;
    smo_accel_x = accelerometer_x / 100 - 5;

    delay(45);
    //    Serial.print(smo_accel_x);
    //    Serial.print("\t");
    //    Serial.print(smo_accel_y);
    //    Serial.println();
}


void transmit()
{

        if (smo_accel_x > -100 && smo_accel_x < 100 && smo_accel_y > -100 && smo_accel_y < 100)
        {
            Serial.write('S');
        }
        else if (smo_accel_x >= 100)
        {
            Serial.write('L');
        }
        else if (smo_accel_x <= -100)
        {
            Serial.write('R');
        }
        else if (smo_accel_y <= -100)
        {
            Serial.write('F');
        }
        else if (smo_accel_y >= 100)
        {
            Serial.write('B');
        }
    
 
}
