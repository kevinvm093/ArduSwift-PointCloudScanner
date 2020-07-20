# ArduSwift-PointCloudScanner

[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/PointCloudFront.png)]( https://youtu.be/yQfqq1JOyl0 "Click here to watch video!")
[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/LidarScannerGIF.gif)](https://youtu.be/yQfqq1JOyl0 "Click here to watch video!")

## Components, Supplies , IDE's
The components attached to the Due & schematic are as follows:
* XCode             - IDE used to program OSX app.
* VS Code           - IDE used to program Arduino 
* Arduino Due       - Controller
* MPU6050           - Gyroscope (Stepper Motor steering wheel)
* 28BYJ-48          - Stepper Motor (Device X-Axis motor)
* ULN2003           - Driver
* TFMini Plus       - LiDAR (Distance measurer)
* 20x4 LCD Screen   - (Interface)
* 9V Battery Supply
* 20Kg Servo Motor  - (Device Y-Axis motor)
* Button
* LED               - Status indicator 


## About this project
###### Arduino Sketch
The device operates in 4 phases:
1. NotReady
2. Ready
3. Scan
4. Done

At start up the device will be initiated in a "NotReady" state & the user will be prompted through the LCD screen to rotate the stepper motor to their desired starting position. The user controls the stepper motor either clockwise or counter clockwise using the gyroscope as a steering wheel. When the gyroscope is tilted left motor rotates left, & vise-versa.

Once the stepper motor is calibrated to the desired starting position the device will switch into "Ready" state and wait for user press button again to initiate scan. During the "Scan" phase the device will begin to scan the room & rotate 180 degrees clockwise from the starting position. As the stepper motor rotates the LiDAR sensor will take a new distance reading every 10 milliseconds. The Arduino then takes that distance measurement as well as 
the current X and Y position and calculates the Spherical Coordinates using a bit of trigonometry. Once spherical coordinates are calculated the Arduino will send new coordinates to the Mac OS application via serial communication.

The device takes approximately 204 reading per Y value, where Y is any value between 55 and 180. Program will automatically switch into the "Done" phase only once its completed its rotation its 180th Y value. 

###### OSX Application 
OS X application is relatively simple it simply listens to the serial port for any data coming in that meets the criteria given. 

Expected data format: 
``` 
<xxx.xxxxx yyy.yyyyy zzz.zzzzz>
``` 
Where the data tranmitted are the coordinates in cartesian format. These coordinates are then parsed and saved into an simd_float3 vector and added to an array of [simd_float3]. 

An observer on the main thread is attached to a flag boolean that is set to true everytime there is new data added into the array of vectors, thus the user is able to watch in realtime the 3D image rendering on the UI as data begins to come in. 

This application uses SceneKit to render every single vector coming, another thing to note is that SIMD is used perform all vector transformations, scales and rotations for speedy performance

## BreadBoard Schematics
[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/breadboard_layout.png)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")

## Future Improvements

###### Device Accuracy
Although its able to produce a relatively "decent" image, ive noticed that some of the readings come out noisy from the TFMini Plus LiDAR... This could be because of how cheap the sensor is, but I'd like to eventually be able to filter out the noisy data so I can have a clean image without having to spend on another LiDAR nor lower the rate in which it takes measurements (apparently the slower it reads the more accurate it is). As of now its rate of measurement is 100 readings per second which is its "standard" speed, but it can go up to 1000Hz if need be.

###### OSX Application
The visuals are cute and all but... Come on its Swift, I'm definietly able to create something a little more visually appealing than its current look. 
