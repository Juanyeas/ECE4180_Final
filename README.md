# ICE CREAM TRUCK  - ROBOT CAR



## Project Idea:

  With parts from our 4180 robot kit kit we decided to incorporate everything we learned of embedded devices  into a small  battery-powered robot that can be remotely controlled via bluetooth with fully motor support and  turning capabilities. It also possesses an LCD screen that displays the status of the robot, speakers that play a tune as the robot moves, and SONAR sensor that relay distance information and stops the robot if anything is detected with within a certain distance in front or behind it. It makes for our very own mininature ice cream truck.


![
](https://github.com/Juanyeas/ECE4180_Final/blob/f79e559e1e9e1f96313151d9817af8d4ba31d226/Picture1.jpg)


### Hardware Parts:
- SparkFun shadow robot chassis
- mbed NXP LPC1768 Microcontroller
- uLCD-144-G2 module
- 2x HC-SR04 sonar sensor
- Adafruit Bluefruit BLE board
- Dual H-bridge driver breakout board
- Speaker
- Breadboard and wires

### Code:
The general concpet behind our code was to use RTOS threads to control the hardware while monitoring changes ftom either the bluetooth or sonar sensors. After the  installation of all hardware conponpents, our next step was to add to correct C++ libraries to our project code which would allow us to run the software related to each of the hardware components.
#### Required Libraries:
  - 
