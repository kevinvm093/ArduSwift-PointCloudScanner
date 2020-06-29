#ifndef KStepper_h
#define KStepper_h

enum Direction 
{ Clockwise, CounterClock, NoMotion };

enum StepType
{ Wave, FullStep, HalfStep };

class KStepper 
{
  public:
  KStepper(int motorPin1, int motorPin2, int motorPin3, int motorPin4);
  void NoMotion();
  void MoveClockwise();
  void MoveCounterClock();

  private:
  // motor pin numbers:
  int motorPin1;
  int motorPin2;
  int motorPin3;
  int motorPin4;

  unsigned int now;
  const int TIME_INTERVAL = 2;
  Direction direction;       // Direction of rotation
};

#endif