#include "DeviceState.h"

DeviceState::DeviceState() 
{
    //Serial.println("DeviceState");
    this->currentState = State::NotReady;
}

State DeviceState::CurrentState()
{
    return this->currentState;
}

void DeviceState::ChangeState(State newState) 
{
    this->currentState = newState;
}