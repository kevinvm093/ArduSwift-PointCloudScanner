#include "KLIidar.h"

KLidar::KLidar()
{
}
uint16_t KLidar::GetDistance()
{
    this->ReadSensorUART();
    return this->distance;
}

uint16_t KLidar::GetStrength() { return this->strength;}
uint16_t KLidar::GetTemperature() { return this->temperature; }

int KLidar::ReadSensorUART(bool flag)
{
    #ifdef DEBUG_MODE
        uint now = micros();
    #endif 
    int counter         = 0;
    byte prevByte       = 0x00;
    byte currentByte    = 0x00;
    bool headersFound   = false;

    while(!Serial3.available());
    while (!headersFound )
    {
        if (Serial3.available())
        {
            counter++;
            currentByte = Serial3.read();

            if (prevByte == FRAME_HEADER && currentByte == FRAME_HEADER) headersFound = true;
            else prevByte = currentByte;
        }

        if (counter > MAX_BYTES_BEFORE_HEADER)
        {
            this->distance = -1;
            this->strength = -1;

            #ifdef DEBUG_MODE
                Serial.println("ERROR: no header");
            #endif

            return ERR_CODE;
        }
    }
    this->distance      = uint16_t(Serial3.read() + Serial3.read());
    this->strength      = uint16_t(Serial3.read() + Serial3.read());
    this->temperature   = uint16_t(Serial3.read() + Serial3.read());
    this->checkSum      = byte(Serial3.read());

    #ifdef DEBUG_MODE
        this->PrintValues();
        Serial.print("Time in micros: \t");Serial.println(micros() - now);
    #endif 
    return SUCCESS;
}

void KLidar::PrintValues() 
{
    Serial.print("distance\t");     Serial.println(this->distance);
    Serial.print("Signal\t");       Serial.println(this->strength);
    Serial.print("temperature\t");  Serial.println(this->temperature);
}
