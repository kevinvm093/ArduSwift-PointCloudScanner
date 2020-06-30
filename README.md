# ArduSwift-PointCloudScanner

[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/PointCloudFront.png)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")
[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/PointCloudTop.png)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")
[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/LidarScannerGIF.gif)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")

###### Components, Supplies , IDE's
The components attached to the Due & schematic are as follows:
* XCode         - IDE used to program OSX app.
* VS Code       - IDE used to program Arduino 
* Arduino Due   - Controller
* MPU6050       - Gyroscope (Stepper Motor steering wheel)
* 28BYJ-48      - Stepper Motor (Device X-Axis)
* ULN2003       - Driver
* TFMini Plus   - LiDAR (Distance measurer)
* 20x4 LCD Screen - (Interface)
* 9V Battery Supply
* 20Kg Servo Motor - (Device Y-Axis)
* Button
* LED - Status indicator 


## About this project
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
<xxx yyy zzz xxx.xxxxx yyy.yyyyy zzz.zzzzz>
Where the integers are the coordinates in cartesian format and the decimal values are in spherical.

The application will then parse in coming data and save them to an array of DataModel which looks like this: 
```
public class DataModel: NSObject {
    
    var xPoint:Float
    var yPoint:Float
    var zPoint:Float
    var sphereVector:SCNVector3
    
    init(withData:[Substring.SubSequence]) {
        
        self.xPoint = Float(withData[0])!
        self.yPoint = Float(withData[1])!
        self.zPoint = Float(withData[2])!
        self.sphereVector = SCNVector3(-Float(withData[3])!,
                                       -Float(withData[5])!,
                                       -Float(withData[4])!)
    }
```
An observer is attached to the array DataModel on the main thread, thus the user is able to watch in realtime the 3D image forming on the MainViewController as data begins to come in. Together each vector makes a point on the Point Cloud rendering a 3D image, this was accomplished with help from Swift's SceneKit Framework.

## BreadBoard Schematics
[![ ](https://github.com/kevinvm093/ArduSwift-PointCloudScanner/blob/master/docs/breadboard_layout.png)](https://youtu.be/2XRfZTwrRYM "Click here to watch video!")

## Future Improvements

###### Device speed
Currently the flow of my device while scanning is:
``` 
1. Begin reading from LiDAR and xmitting data.
2. Sweep clockwise from starting point to 180 degrees.
3. Stop reading from LiDAR and xmitting data.
4. Rotate counter clockwise.
5. Increment Y axis. 
6. Repeat steps: 1-5 until Y reaches 180.
```
As you can probably imagine this is a very slow approach, a much better one would be:
``` 
1. Begin reading from LiDAR and xmitting data.
2. Sweep clockwise from starting point to 180 degrees.
3. Increment Y axis. 
4. Rotate counter clockwise.
5. Repeat steps: 1-4 until Y reaches 180.
``` 

By doing so I would cut the time by half. But my results are also distorted appear distorted as such creating a double or "ghost" of me just a few degrees away from me on the X-axis.
[add image here]
Any feedback on how to correct this would be much appreciated lol.... I think it might have to do with the sensor, but I'm sure theres a way I can calculate an offset value to I get the nice undistorted image when ran the long way.

I also would like to atempt running my code on a beefier stepper motor with a little more RPM power, I think I would find that to be highly affective.

###### Device Accuracy
Although its able to produce a relatively "decent" image, ive noticed that some of the readings come out noisy from the TFMini Plus LiDAR... This could be because of how cheap the sensor is, but I'd like to eventually be able to filter out the noisy data so I can have a clean image without having to spend on another LiDAR nor lower the rate in which it takes measurements (apparently the slower it reads the more accurate it is). As of now its rate of measurement is 100 readings per second which is its "standard" speed, but it can go up to 1000Hz if need be.

###### OSX Application
The visuals are cute and all but... Come on its Swift, I'm definietly able to create something a little more visually appealing than its current look. 
