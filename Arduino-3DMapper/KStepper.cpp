
#include "Arduino.h"
#include "KStepper.h"

KStepper::KStepper(int motorPin1, int motorPin2, int motorPin3, int motorPin4) 
{
  // Set motor pins as output and direction to NoMotion.
    this->motorPin1     = motorPin1;
    this->motorPin2     = motorPin2;
    this->motorPin3     = motorPin3;
    this->motorPin4     = motorPin4;

    pinMode(this->motorPin1, OUTPUT);
    pinMode(this->motorPin2, OUTPUT);
    pinMode(this->motorPin3, OUTPUT);
    pinMode(this->motorPin4, OUTPUT);

    this->direction     = Direction::NoMotion;  
}

void KStepper::NoMotion() 
{
    digitalWrite(this->motorPin1, LOW);
    digitalWrite(this->motorPin2, LOW);
    digitalWrite(this->motorPin3, LOW);
    digitalWrite(this->motorPin4, LOW);
    delayMicroseconds(100);
}

// Moves motors 4 steps clockwise 
void KStepper::MoveClockwise()
{
  //1000
  digitalWrite(this->motorPin1, HIGH);
  digitalWrite(this->motorPin2, LOW);
  digitalWrite(this->motorPin3, LOW);
  digitalWrite(this->motorPin4, LOW);
  this->now = millis();
  while(millis() - now < this->TIME_INTERVAL);

  //0100
  digitalWrite(this->motorPin1, LOW);
  digitalWrite(this->motorPin2, HIGH);
  digitalWrite(this->motorPin3, LOW);
  digitalWrite(this->motorPin4, LOW);
  this->now = millis();
  while(millis() - now < this->TIME_INTERVAL);

  //0010
  digitalWrite(this->motorPin1, LOW);
  digitalWrite(this->motorPin2, LOW);
  digitalWrite(this->motorPin3, HIGH);
  digitalWrite(this->motorPin4, LOW);
  this->now = millis();
  while(millis() - now < this->TIME_INTERVAL);

  //0001
  digitalWrite(this->motorPin1, LOW);
  digitalWrite(this->motorPin2, LOW);
  digitalWrite(this->motorPin3, LOW);
  digitalWrite(this->motorPin4, HIGH);
  this->now = millis();
  while(millis() - now < this->TIME_INTERVAL);
}

// Moves motors 4 steps counter-clockwise
void KStepper::MoveCounterClock()
{
  //0001
  digitalWrite(this->motorPin1, LOW);
  digitalWrite(this->motorPin2, LOW);
  digitalWrite(this->motorPin3, LOW);
  digitalWrite(this->motorPin4, HIGH);
  this->now = millis();
  while(millis() - now < this->TIME_INTERVAL);

  //0010
  digitalWrite(this->motorPin1, LOW);
  digitalWrite(this->motorPin2, LOW);
  digitalWrite(this->motorPin3, HIGH);
  digitalWrite(this->motorPin4, LOW);
  this->now = millis();
  while(millis() - now < this->TIME_INTERVAL);

  //0100
  digitalWrite(this->motorPin1, LOW);
  digitalWrite(this->motorPin2, HIGH);
  digitalWrite(this->motorPin3, LOW);
  digitalWrite(this->motorPin4, LOW);
  this->now = millis();
  while(millis() - now < this->TIME_INTERVAL);

  //1000
  digitalWrite(this->motorPin1, HIGH);
  digitalWrite(this->motorPin2, LOW);
  digitalWrite(this->motorPin3, LOW);
  digitalWrite(this->motorPin4, LOW);
  this->now = millis();
  while(millis() - now < this->TIME_INTERVAL);
}
