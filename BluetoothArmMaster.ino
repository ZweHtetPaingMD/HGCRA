#include <Wire.h>

const int flexPin = A0;
const int MPU6050_address = 0x68;
int ACCEL_XOUT_H, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L;
int GYRO_XOUT_H, GYRO_XOUT_L, GYRO_YOUT_H, GYRO_YOUT_L, GYRO_ZOUT_H, GYRO_ZOUT_L;
double ACCEL_XOUT, ACCEL_YOUT, ACCEL_ZOUT, GYRO_XOUT, GYRO_YOUT, GYRO_ZOUT, dt, start, finish;
double gx, gy, gz, elapsed, angle_X, angle_Y, angle_Z, X, Y, Z;
const double pi = 3.14159265, alpha = 0.96;
char tmp_str[7];
int16_t flexValue;
int16_t fX, fY, fZ;

char* convert_int16_to_str(int16_t i);

void control();
void transmit();

void setup()
{
    Wire.begin();
    Serial.begin(9600);

    Wire.beginTransmission(MPU6050_address);
    Wire.write(0x6B);
    Wire.write(B00000000);  //enables wakeup mode
    Wire.endTransmission();
}

void loop()
{
    control();
    transmit();
}


void control()
{
    start = millis();

    Wire.beginTransmission(MPU6050_address);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_address, 2);
    ACCEL_XOUT_H = Wire.read();
    ACCEL_XOUT_L = Wire.read();

    ACCEL_XOUT_H = ACCEL_XOUT_H << 8;
    ACCEL_XOUT = ((ACCEL_XOUT_H + ACCEL_XOUT_L) / 16384.00) - 0.02; //Last value is offset value


    Wire.beginTransmission(MPU6050_address);
    Wire.write(0x3D);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_address, 2);
    ACCEL_YOUT_H = Wire.read();
    ACCEL_YOUT_L = Wire.read();

    ACCEL_YOUT_H = ACCEL_YOUT_H << 8;
    ACCEL_YOUT = ((ACCEL_YOUT_H + ACCEL_YOUT_L) / 16384.00) - 0.025; //Last value is offset value


    Wire.beginTransmission(MPU6050_address);
    Wire.write(0x3F);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_address, 2);
    ACCEL_ZOUT_H = Wire.read();
    ACCEL_ZOUT_L = Wire.read();

    ACCEL_ZOUT_H = ACCEL_ZOUT_H << 8;
    ACCEL_ZOUT = ((ACCEL_ZOUT_H + ACCEL_ZOUT_L) / 16384.00) - 0.005; //Last value is offset value


    Wire.beginTransmission(MPU6050_address);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_address, 2);
    GYRO_XOUT_H = Wire.read();
    GYRO_XOUT_L = Wire.read();

    GYRO_XOUT_H = GYRO_XOUT_H << 8;
    GYRO_XOUT = ((GYRO_XOUT_H + GYRO_XOUT_L) / 131.00) - 0.791; //Last value is offset value
    gx = (GYRO_XOUT * dt) * pi / 180.00;


    Wire.beginTransmission(MPU6050_address);
    Wire.write(0x45);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_address, 2);
    GYRO_YOUT_H = Wire.read();
    GYRO_YOUT_L = Wire.read();

    GYRO_YOUT_H = GYRO_YOUT_H << 8;
    GYRO_YOUT = ((GYRO_YOUT_H + GYRO_YOUT_L) / 131.00) + 0.291; //Last value is offset value
    gy = (-1 * (GYRO_YOUT) * dt) * pi / 180.00;


    Wire.beginTransmission(MPU6050_address);
    Wire.write(0x47);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_address, 2);
    GYRO_ZOUT_H = Wire.read();
    GYRO_ZOUT_L = Wire.read();

    GYRO_ZOUT_H = GYRO_ZOUT_H << 8;
    GYRO_ZOUT = ((GYRO_ZOUT_H + GYRO_ZOUT_L) / 131.00) - 0.7913; //Last value is offset value
    gz = (GYRO_ZOUT * dt) * pi / 180.00;

    angle_X = atan2(ACCEL_YOUT, ACCEL_ZOUT);
    angle_Y = atan2(ACCEL_XOUT, ACCEL_ZOUT);
    //angle_Z = atan2(ACCEL_YOUT, ACCEL_XOUT);

    X = alpha * (X + gx) + (1.00 - alpha) * angle_X; // pitch angles seem incorrect
    Y = alpha * (Y + gy) + (1.00 - alpha) * angle_Y; // roll angles seem incorrect
    Z = (Z + gz);
    delay(45);

    fX = X * 35;
    fY = Y * 35;
    fZ = Z * 35;
    //    Serial.println(X);
    //Serial.print("\t");
    //    Serial.println(Y);
    //Serial.print("\t");
    //    Serial.println(Z);

    finish = millis();
    elapsed = finish - start;
    dt = elapsed / 1000.00;
    start = 0.00;
    elapsed = 0.00;

    flexValue = analogRead(flexPin);

}

void transmit()
{
    Serial.write(convert_int16_to_str(flexValue));
    Serial.write(',');
    Serial.write(convert_int16_to_str(fX));
    Serial.write(',');
    Serial.write(convert_int16_to_str(fZ));
    Serial.write('\n');
    delay(45);
}


char* convert_int16_to_str(int16_t i)
{
    sprintf(tmp_str, "%6d", i);
    return tmp_str;
}
