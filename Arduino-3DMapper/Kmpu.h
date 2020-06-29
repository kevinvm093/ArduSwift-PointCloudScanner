
/* Arduino 3D imaging LIDAR scanner
    written by : Kevin Vallejo in 2020
    www.github.com/kevinvm093
Kmpu - MPU6050 reader.
    
*/

#ifndef Kmpu_h
#define Kmpu_h

#include "KStepper.h"
#include "Arduino.h"


#define SUCCESS         0
#define DATA_LENGTH     6
#define MPU_ADDR        0x68
#define PWR_MGMT_1      0x6B
#define DEVICE_RESET    0x07
#define ACCEL_CONFIG    0x1C
#define ACCEL_START     0x10
#define GYRO_CONFIG     0x1B
#define GYRO_START      0x08
#define ACCEL_XOUT_H    0x3B
#define GYRO_XOUT_H     0x43
#define MPU6050_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD



struct Gyro 
{
    int16_t x;
    int16_t y;
    int16_t z;
    long    xCalc;
    long    yCalc;
    long    zCalc;
};

struct Accel 
{
    int16_t x;
    int16_t y;
    int16_t z;
    float   roll;
    float   pitch;
    long    forceVector; //is the inertial force vector 
};

struct Angle 
{
    float roll;
    float pitch;
    float rollOutput;
    float pitchOutput;
};

class Kmpu 
{
    public:

    // Public functions
    Kmpu();
    Direction   findAngle();
    bool        calibrateMPU();
    void        PrintValues();
    void        ReadData();
    
    private:
    // Constants
    const int       COUNTER     = 1000;
    const double    LEVELED     = 10.00;
    const double    TURN        = 60.00;
    const double    RAD_2_DEG   = 0.0000611;
    const double    G_RADIANS   = 0.000001066;

    // private variables
    Gyro            gyro;
    Accel           accel;
    Angle           angle;
    int16_t         temp;
    bool            setGyroAngles;

    // Private functions
    double           ToDegrees(float value);
};

#endif