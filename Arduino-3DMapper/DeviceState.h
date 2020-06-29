#ifndef DeviceState_h
#define DeviceState_h


enum State  { NotReady, Calibrate, Ready, Scan, Done};

class DeviceState 
{
    public:
    DeviceState(); // Constructor sets intial value to "NotReady"
    State CurrentState(); // Getter funtion
    void ChangeState(State newState); // Setter funtion

    private:
    State currentState;
};

#endif