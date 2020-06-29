#include "Kmpu.h"
#include "KLCD.h"
#include <Servo.h>
#include "KLIidar.h"
#include "KStepper.h"
#include "Controls.h"
#include <DueTimer.h>
#include "DeviceState.h"

/* ---------------------------------
            Define pins
 * --------------------------------- 

Motor controller - 
    Power - 9V battery.
    N1 - Motor control A on pin 2.
    N2 - Motor control B on pin 3.
    N3 - Motor control C on pin 4.
    N4 - Motor control D on pin 5.

    BTN - Button on pin 22.
    LED - Blue LED on pin 9.

Servo motor -
    Power - 7.4V battery.
    SERVO - servo motor signal on pin 25.
*/
const byte N1       = 2;
const byte N2       = 3;
const byte N3       = 4;
const byte N4       = 5;
const byte BTN      = 22;
const byte LED      = 9;
const byte SERVO    = 25;
/* ---------------------------------
            Define constants
 * --------------------------------- 

Motor controller - 
    Power - 9V battery.
    N1 - Motor control A on pin 2.
    N2 - Motor control B on pin 3.
    N3 - Motor control C on pin 4.
    N4 - Motor control D on pin 5.

    BTN - Button on pin 22.
    LED - Blue LED on pin 9.

Servo motor -
    Power - 7.4V battery.
    SERVO - servo motor signal on pin 25.
*/

const int  STEPS            = 256;
const byte XY_MIN           = 0;
const byte XY_MAX           = 180;
const byte SERVO_MAX        = 125;
const byte SERVO_MIN        = 0;
const byte MIN_AZIMUTH      = 0;
const byte MAX_AZIMUTH      = 204;
const byte DEBOUNCE_TIME    = 50;

/* ---------------------------------
            Status keppers
 * --------------------------------- 
 
  currentX      - Gets incremented (or decrimented) every time LIDAR takes measurement.
  currentY      - Only gets incremented once stepper motor has taken 1024 steps in either direction.
  incrementX    - Flat that indicates if currentX should be incremented or decrimented.
  readMPU       - Flag that indicates MPU sensor is ready to be read.
  btnState      - used for interrupt 
  lastBtnState  - Validates previous buttons state
  ledState      - state in which LED is currently on. 
  lastPress     - Time in which button was last toggled.
*/
int currentX;
int currentY;
bool incrementX;
volatile bool readMPU;
volatile bool btnState;
byte lastBtnState;
bool ledState;
long unsigned int lastPress;

/* ---------------------------------
        Intialize components
 * --------------------------------- */
Kmpu                mpu;
Servo               servo;
KLidar              lidar;
KStepper            kStepper(N1, N2, N3, N4);
State               currentState;
DeviceState         deviceState;

