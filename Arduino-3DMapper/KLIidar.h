#ifndef KLidar_h
#define KLidar_h
#include <Wire.h>

//#define DEBUG_MODE

#define LIDAR_BAUD_RATE              115200
#define LIDAR_ADDR                   0x10
#define DATA_LENGTH                  0x04
#define FRAME_HEADER                 0X59
#define FRAME_SIZE                   7
#define ERR_CODE                    -1
#define SUCCESS                     0
#define MAX_BYTES_BEFORE_HEADER     20
#define CMND_LEN_INDEX              1

class KLidar
{
   public:
    KLidar();
    uint16_t GetDistance();
    uint16_t GetStrength();
    uint16_t GetTemperature();

   private:
    uint16_t    header;
    uint16_t    distance;
    uint16_t    strength;
    uint16_t    temperature;
    byte        checkSum;

    void PrintValues();
    bool ChangeFrameRateUART();
    int ReadSensorUART(bool flag = false);
};

#endif