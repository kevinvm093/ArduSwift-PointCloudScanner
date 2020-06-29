/* ---------------------------------
    Controls namespace decalaration
 * ---------------------------------*/

#ifndef Controls_h
#define Controls_h

namespace Controls
{
    void NotReady();
    void Calibrate();
    void Ready();
    void Scan();
    void Done();
    void ProcessData(uint16_t dist);
    float Map(float x, float in_min, float in_max, float out_min, float out_max);
    void ResetServo();
} // namespace Controls

#endif