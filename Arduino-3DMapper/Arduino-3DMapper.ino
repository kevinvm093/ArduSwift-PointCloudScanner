#include "Config.h"
/* ---------------------------------
 * --------------------------------- */
void setup()
{
    // I2C & Serial connections
    Wire.begin();
    Wire.setClock(400000);

    Serial.begin(115200);
    Serial3.begin(115200); 

    // LCD
    KLCD::Configure();
 
    // IMU
    mpu.calibrateMPU();
    Timer6.attachInterrupt(ISR_fetchMPU).setFrequency(250);

    // Servo
    servo.write(SERVO_MIN);
    servo.attach(SERVO);

    // LIDAR
    Timer1.attachInterrupt(ISR_fetchLidar).setFrequency(100);

    // LED
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    ledState = false;
    Timer7.attachInterrupt(ISR_LED).setFrequency(15);

    // button
    pinMode(BTN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BTN), ISR_btnToggle, CHANGE);

    currentState = deviceState.CurrentState();

    unsigned long now = millis();
    while(millis() - now < 1000);
}

// Loop checks which state device is in.
// Once state has changed program will return to loop and update to new state.
void loop()
{
    KLCD::DisplayStatus(currentState);
    switch (currentState)
    {
    case State::NotReady:
         Controls::NotReady();
         break;

    case State::Calibrate:
         Controls::Calibrate();
         break;

    case State::Ready:
         Controls::Ready();
         break;

    case State::Scan:
         Controls::Scan();
         break;

    case State::Done:
         Controls::Done();
         break;
    }
    currentState = deviceState.CurrentState(); // Record new state
}

// ================================================================
// ===                   INTERRUPTS                             ===
// ================================================================
void ISR_btnToggle() { btnState = true;} // Button interrupt

void ISR_fetchMPU() { Timer6.stop(); readMPU = true;} // MPU interrupt

void ISR_LED(){ ledState = !ledState; digitalWrite(LED, ledState); }

void checkBtnState(State newState)
{
    if ((millis() - lastPress) > DEBOUNCE_TIME && btnState) 
    { // update last press.
        lastPress = millis(); 
        if (digitalRead(BTN) == 0 && lastBtnState == 1) 
        { // if button is pressed and was released last change
            deviceState.ChangeState(newState);
            lastBtnState = 0;
        }
        else if (digitalRead(BTN) == 1 && lastBtnState == 0)
        { // reset lastBtnState
            lastBtnState = 1;
        }
        btnState = false;
    }
}

// Interrupt that reads from LIDAR
void ISR_fetchLidar()
{ 
    // implement function that reconfigures lidar readings
    Controls::ProcessData(lidar.GetDistance());
    if (incrementX == true) currentX++;
    else currentX--;
}

// ================================================================
// ===                   MAIN CONTROLS                          ===
// ================================================================
namespace Controls
{
    // Function requesting user to calibrate stepper motor to proper position
    void NotReady()
    {
        // do nothing while button hasn't toggled
        do {checkBtnState(State::Calibrate);} while (currentState == deviceState.CurrentState());
    }

    // Calibration method moves stepper motor to MPU's groove.
    void Calibrate()
    {
        Direction newDirection = Direction::NoMotion;
        Timer6.start(); // MPU interrupt enabled.
        do
        { // While button hasn't toggled
            if (readMPU == true) 
            { // checks timer6 interrupt
                newDirection = mpu.findAngle();
                readMPU = false;
                Timer6.start();
            }
            // Decides which direction motors should move.
            if (newDirection == Direction::NoMotion)          { kStepper.NoMotion();          }
            else if (newDirection == Direction::Clockwise)    { kStepper.MoveClockwise();     }
            else if (newDirection == Direction::CounterClock) { kStepper.MoveCounterClock();  }

            checkBtnState(State::Ready);
        } while (currentState == deviceState.CurrentState());
        Timer6.stop(); // MPU interrupt disabled.
    }

    // Read state waits for button toggle before starting scan.
    void Ready()
    {
        Timer7.start();
        // While button hasn't toggled
        do { checkBtnState(State::Scan); } while (currentState == deviceState.CurrentState());
        Timer7.stop();
    }

// Does 180 degree scans on both X & Y - axis while taking readings from LIDAR sensor.
    void Scan()
    {     
        digitalWrite(LED, LOW);
        unsigned long now = millis();
        while(millis() - now < 3000); // wait 3 seconds before starting scan.
        digitalWrite(LED, HIGH); // LED indicates scan has begun.
        
        currentX = 0;
        for (int servoPos = SERVO_MIN; servoPos < SERVO_MAX; servoPos++)
        {
            servo.attach(25);
            servo.write(servoPos);
            delay(10);
            servo.detach();
            
            // If statements sweeps stepper motor from side to side depending on y-axis
            // Stepper takes 1024 steps for one full rotation. 
            // MoveClockwise() & MoveCounterClock() methods steps 4 times per function call. 
            // Thus 256 * 4  = 512 steps (half rotation)
            if(currentY % 2 == 0) 
            { 
                incrementX = true;
                int counter = 0;
                Timer1.start();
                while (counter < STEPS)
                {    
                    kStepper.MoveClockwise();
                    counter++;
                }
            }
            else 
            {
                incrementX = false;
                int counter = 0;
                Timer1.start();
                while (counter < STEPS)
                {    
                    kStepper.MoveCounterClock();
                    counter++;
                }
            }
            Timer1.stop();
            currentY++;
        }
        Timer1.stop(); // LIDAR interrupt diabled.
        kStepper.NoMotion();
        deviceState.ChangeState(State::Done);
        //Serial.print("Total time for scan:\t"); Serial.print((millis() - now) / 60000); Serial.print("min");
    }

    void Done()
    {
        Timer7.start();
        ResetServo();
        while(1);
    }

    // Converts cartesian coordinates into spherical and transmit via serial 
    void ProcessData(uint16_t dist)
    {
        float az = (Map(currentX, MIN_AZIMUTH, MAX_AZIMUTH, XY_MIN, XY_MAX) * (PI / 180));
        float el = (180 - SERVO_MAX + currentY)  * (PI / 180);

        double pointX = dist * sin(el) * cos(az);
        double pointY = dist * sin(el) * sin(az);
        double pointZ = dist * cos(el);

        Serial.print("<" + String(currentX) + " " + String(currentY) + " " + String(dist) + " ");
        Serial.print(String(pointX, 5) + " " + String(pointY, 5) + " " + String(pointZ, 5) + ">");
    }

    // LIDAR x coordinates are from 0 - 204, Map() will translate those coords to values between 0 - 180.
    // this one differs from the arduino standard map() function in that it returns a float.
    float Map(float x, float in_min, float in_max, float out_min, float out_max) 
    {return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;}

// Resets servo pack to intitial (SERVO_MIN) position. Not in use yet.
    void ResetServo()
    {
        byte servoPos = SERVO_MAX;
        int stepCounter = 0;

        if (servoPos-- != SERVO_MIN)
        {
            servo.attach(SERVO);
            servo.write(servoPos);
            delay(15);
            servo.detach();
        }

        kStepper.NoMotion();
    }
} // namespace Controls
