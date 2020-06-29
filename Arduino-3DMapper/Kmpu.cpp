#include "Kmpu.h"
#include <Wire.h>


Kmpu::Kmpu()
{
    this->gyro = Gyro();
    this->accel = Accel();
    this->angle = Angle();
    this->setGyroAngles = false;
}

bool Kmpu::calibrateMPU()
{
    // TODO: add auto reset functionality.
    
    //Set MPU clock to wake up.
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(PWR_MGMT_1);
    Wire.write(MPU6050_RA_XG_OFFS_TC);
    Wire.endTransmission();

    // Set Accel
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(ACCEL_CONFIG);
    Wire.write(ACCEL_START);
    Wire.endTransmission();

    // Set Gyro
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(GYRO_CONFIG);
    Wire.write(GYRO_START);
    Wire.endTransmission();

    for (int counter = 0; counter < COUNTER; counter++)
    { // averages out MPU's readings
        this->ReadData();
        this->gyro.xCalc += gyro.x;
        this->gyro.yCalc += gyro.y;
        this->gyro.zCalc += gyro.z;
        delay(3);
    }

    this->gyro.xCalc /= COUNTER;
    this->gyro.yCalc /= COUNTER;
    this->gyro.zCalc /= COUNTER;

    return 0;
}

Direction Kmpu::findAngle()
{
    this->ReadData();

    // Angles measured by Gyro scope.
    this->gyro.x -= gyro.xCalc;
    this->gyro.y -= gyro.yCalc;
    this->gyro.z -= gyro.zCalc;

    this->angle.pitch += gyro.x * 0.0000611;
    this->angle.roll  += gyro.y * 0.0000611; 

    this->angle.pitch += angle.roll  * sin(gyro.z * 0.000001066);
    this->angle.roll  -= angle.pitch * sin(gyro.z * 0.000001066);

    // Angles measured by accelerometer.

    // Calculate the intertial force vector.
    //this->accel.forceVector = sqrt(pow(this->accel.x, 2) + pow(this->accel.y, 2) + pow(this->accel.z, 2));
   this->accel.forceVector = sqrt((this->accel.x * this->accel.x) + (this->accel.y * this->accel.y) + (this->accel.z * this->accel.z));
   
    this->accel.pitch = asin((float)accel.y / accel.forceVector) *  57.296;
    this->accel.roll  = asin((float)accel.x / accel.forceVector) * -57.296;

    this->accel.pitch -= 0.0;
    this->accel.roll  -= 0.0;

    if (setGyroAngles)
    {
        this->angle.pitch = this->angle.pitch * 0.9996 + this->accel.pitch * 0.0004;
        this->angle.roll  = this->angle.roll  * 0.9996 + this->accel.roll  * 0.0004;
    }
    else
    {
        //Set the gyro pitch angle equal to the accelerometer pitch angle
        this->angle.pitch   = this->accel.pitch;
        this->angle.roll    = this->accel.roll;
        this->setGyroAngles = true;
    }

    this->angle.pitchOutput = this->angle.pitchOutput * 0.9 + this->angle.pitch * 0.1;
    this->angle.rollOutput = this->angle.rollOutput  * 0.9 + this->angle.roll  * 0.1;
    
    
   // Serial.print("Angle:\t"); Serial.println(this->angle.pitchOutput);

    if (this->angle.pitchOutput >= -(this->LEVELED) && this->angle.pitchOutput <= this->LEVELED)
    {
        return Direction::NoMotion;
    }
    else if (this->angle.pitchOutput < -(this->LEVELED))
    {
        return Direction::Clockwise;
    }
    else if (this->angle.pitchOutput > this->LEVELED)
    {
        return Direction::CounterClock;
    }
    
}

double Kmpu::ToDegrees(float value) { return value / (PI / 180); }

void Kmpu::ReadData()
{
    // Read Accel
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(ACCEL_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADDR, DATA_LENGTH);
    while (Wire.available() < DATA_LENGTH);
    this->accel.x = Wire.read() << 8 | Wire.read();
    this->accel.y = Wire.read() << 8 | Wire.read();
    this->accel.z = Wire.read() << 8 | Wire.read();
   
    // Read Gyro
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(GYRO_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADDR, DATA_LENGTH);
    while (Wire.available() < DATA_LENGTH);
    this->gyro.x = Wire.read() << 8 | Wire.read();
    this->gyro.y = Wire.read() << 8 | Wire.read();
    this->gyro.z = Wire.read() << 8 | Wire.read();
}

void Kmpu::PrintValues()
{
    Serial.println("------------------------------------------");
    Serial.print("accel.x :\t"); Serial.print(this->accel.x); Serial.print("\tgyro.x :\t"); Serial.println(this->gyro.x);
    Serial.print("accel.y :\t"); Serial.print(this->accel.y); Serial.print("\tgyro.x :\t"); Serial.println(this->gyro.y);
    Serial.print("accel.z :\t"); Serial.print(this->accel.z); Serial.print("\tgyro.x :\t"); Serial.println(this->gyro.z);
    Serial.println("------------------------------------------");
}


