# ArduSwift-PointCloudScanner

## What it does
Arduino sketch that works with OSX application written in Swift 5.2 to create a point cloud image rendered in 3D.

Here are a few examples of the rendered image:

[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/PointCloudFront.png)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")
[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/PointCloudTop.png)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")
[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/LidarScannerGIF.gif)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")

## How it works

###### Arduino Due
The components attached to the Due & schematic are as follows:
* MPU6050   - Gyroscope
* 28BYJ-48  - Stepper Motor
* ULN2003   - Driver
* TFMini Plus - LiDAR
* 20x4 LCD Screen
* 9V Battery Supply
* 20Kg Servo Motor
* Button
* LED

[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/breadboard_layout.png)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")

Once user calibrates the stepper motor to point to their desire position (using the gyroscope), the device will begin to scan the room from 0 to 180 degrees. 
As the stepper motor rotates the LiDAR sensor will take a new distance reading every 10 milliseconds. The arduino then takes that distance measurement as well as 
the current x and y position and calculates the Sperical Coordinates using a little bit of trigonometry. Once spherical coordinates are calculated the Arduino will 
send that data to Mac OS application via serial communication. 

###### OSX Application
To be continued.... 
