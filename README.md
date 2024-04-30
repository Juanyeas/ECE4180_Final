# ICE CREAM TRUCK  - ROBOT CAR
  Group Members: Moses Goldsby, Hayes Rollins, and Thomas Chan



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
-  2x DC motors
- Speaker
- Breadboard and wires
  #### Assembly:
  DC motors:
  | Mbed LPC1768 | H-Bridge motor driver  | DC motors | Battery pack |
  |------------- | ---------------------- | --------- | ------------ |
  | -- | Vm | |  5V(+) | 
  | Gnd |Gnd | | - | 
  | -- | Vcc | |
  | p21 | PWMB | |
  | p5 | BIN2 | |
  | p6 | BIN1 | |
  | P8 | AIN1 | |
  | p7 | AIN2 | |
  | p22 | PWMA | |
  | Vout | /STBY | |
  | | AOUT1 | left-red(+)
  | | AOUT2 | left-blk(-)
  | | BOUT2 | right-blk(-)
  | | BOUT1 | right-red(+)

uLCD:
|Mbed LPC1768 | uLCD  Header| uLCD Cable| Battery Pack |
|------------ | ----------- | --------- | ----------- |
| - | 5V | 5V | 5V(+) |
| Gnd | Gnd | Gnd | |  
| TX = p13 | RX | TX |
| RX = p14 | TX | RX |
| p12 | Reset | Reset | 

Bluetooth module:
|Mbed LPC1768 | Adafruit BLE| Battery Pack |
|------------ | ----------- |  ----------- |
|Gnd | Gnd | | 
| - | Vin | 5V(+) |
| - | RTS | |
| Gnd | CTS | 
| p9(RX) | TXO |
| p10(TX) | RXI |

Sonar Sensors:
|Mbed LPC1768 | uLCD  Header| uLCD Cable| Battery Pack |
|------------ | ----------- | --------- | ----------- |


### Code:
The general concpet behind our code was to use RTOS threads to control the hardware while monitoring changes from both the bluetooth controller and sonar sensors. After the  installation of all hardware conponpents, our next step was to add to correct C++ libraries to our project code which would allow us to run the software related to each of the hardware components. Many of the source files are include in mbed's main library but a few of the hardware components required additional libraries to function correctly.
#### Required Libraries:
  - Mbed: [mbed RTOS 2](http://mbed.org/users/mbed_official/code/mbed-rtos/#ee87e782d34f)
  - LCD: [4DGL-uLCD](https://os.mbed.com/users/4180_1/code/4DGL-uLCD-SE/#2cb1845d768165993c6c4e2f245a16ea983a8c1f) and [uLCD Fonts](http://mbed.org/users/dreschpe/code/LCD_fonts/#d0b7d7bf1f56)
  - Motors: [Motor Drvier](https://os.mbed.com/users/littlexc/code/Motordriver/#3110b9209d3ccd8b17a42e80102592979dbcc549)

    

