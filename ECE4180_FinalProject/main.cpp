#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"
#include "motordriver.h"

BusOut myled(LED1,LED2,LED3,LED4);
Serial blue(p9,p10);
Motor left (p21, p6, p5, 0);
Motor right (p22, p8, p7, 0);
uLCD_4DGL uLCD(p13, p14, p12);
Mutex mux;
DigitalOut backtrigger(p28);
DigitalIn backecho(p27);
DigitalOut fronttrigger(p29);
DigitalIn frontecho(p30);
PwmOut speaker(p23);
int frontdistance = 0;
int frontcorrection = 0;
int backdistance = 0;
int backcorrection = 0;
Timer frontsonar;
Timer backsonar;
int mode = 1;
double s = 0; 

//Speaker Helper Function
void playNote(float note, int duration)
{
    mux.lock();
    if (note != 0)
    {
        speaker.period(1.0/note);
        speaker.write(0.50f);
    }
    else 
    {
        speaker.write(0.00f);
    }
    mux.unlock();

    Thread::wait(duration);
}

//Sonar Helper Function
void dist(int distance)
{
    // if distance is less 10 cm stop
    if (distance < 10) {
        left.coast();
        right.coast();
        playNote(0, 1000);
        wait_ms(1000);
        // if in auto mode change direction
        if (mode == 3) {
            s = -1 * s;
            left.speed(s);
            right.speed(s);
        }
        if (mode == 4) { 
            left.speed(0.5);
            right.speed(0.5);
            wait_ms(2000);
            left.speed(-0.5);
            right.speed(-0.25);
            wait_ms(2000);
            left.speed(-0.5);
            right.speed(-0.5);
        }
    }
}
//Sonar Helper function
void backdist(int distance)
{
    // if distance is less 10 cm stop
    if (distance < 10) {
        left.coast();
        right.coast();
        playNote(0, 1000);
        wait_ms(1000);
        // if distance is less 10 cm stop
        if (mode == 3) {
            s = -1 * s;
            left.speed(s);
            right.speed(s);
        }
        // if in auto mode change direction
        if (mode == 4) { 
            left.speed(-0.5);
            right.speed(-0.5);
            wait_ms(2000);
            left.speed(0.5);
            right.speed(1);
            wait_ms(2000);
            left.speed(0.5);
            right.speed(0.5);
        }
    }
}

//uLCD Update Thread prints the current mode and motor action to uLCD every second
void uLCD_thread(void const *args) {
    while(1) {
        mux.lock();
        uLCD.cls();
        if (left.state() == 2) {
            uLCD.printf("\n Stopped");
        } 
        else if (left.state() < 0) {
            uLCD.printf("\n Forward");
        } 
        else {
            uLCD.printf("\n Reversing");
        }
        uLCD.printf("\n Current Mode: %d", mode);
        mux.unlock();
        Thread::wait(1000); 
    }
}

//Sonar thread gets the current distance from the front sensor
void frontsonar_thread(void const *args) {
        frontsonar.reset();
        // measure actual software polling timer delays
        // delay used later in time correction
        // start timer
        frontsonar.start();
        // min software polling delay to read echo pin
        while (frontecho==2) {};
        // stop timer
        frontsonar.stop();
        // read timer
        frontcorrection = frontsonar.read_us();
    while (true)
    {
        // trigger sonar to send a ping
        fronttrigger = 1;
        frontsonar.reset();
        wait_us(10.0);
        fronttrigger = 0;
        //wait for echo high
        while (frontecho==0) {};
        //echo high, so start timer
        frontsonar.start();
        //wait for echo low
        while (frontecho==1) {};
        //stop timer and read value
        frontsonar.stop();
        //subtract software overhead timer delay and scale to cm
        frontdistance = (frontsonar.read_us()-frontcorrection)/58.0;
        //Calls dist to check if it needs to stop;
        dist(frontdistance);
        //wait so that any echo(s) return before sending another ping
        wait(0.2);
        Thread::wait(500);
    }
}

//Sonar thread gets the current distance from the front sensor
void backsonar_thread(void const *args) {
        backsonar.reset();
        // measure actual software polling timer delays
        // delay used later in time correction
        // start timer
        backsonar.start();
        // min software polling delay to read echo pin
        while (backecho==2) {};
        // stop timer
        backsonar.stop();
        // read timer
        backcorrection = backsonar.read_us();
    while (true)
    {
        // trigger sonar to send a ping
        backtrigger = 1;
        backsonar.reset();
        wait_us(10.0);
        backtrigger = 0;
        //wait for echo high
        while (backecho==0) {};
        //echo high, so start timer
        backsonar.start();
        //wait for echo low
        while (backecho==1) {};
        //stop timer and read value
        backsonar.stop();
        //subtract software overhead timer delay and scale to cm
        backdistance = (backsonar.read_us()-backcorrection)/58.0;
        //Calls dist to check if it needs to stop;
        dist(backdistance);
        //wait so that any echo(s) return before sending another ping
        wait(0.2);
        Thread::wait(500);
    }
}

//Speaker thread loops the daisy bell song
void speaker_thread(void const *args) {
     while (true)
    {
        playNote(391.995, 750); // G4
        playNote(329.628, 750); // E4
        playNote(261.626, 750); // C4
        playNote(195.998, 750); // G3
        playNote(220, 250); // A3
        playNote(246.942, 250); // B3
        playNote(261.626, 250); // C4
        playNote(220, 500); // A3
        playNote(261.626, 250); // C4
        playNote(195.998, 1000); // G3
        playNote(0, 500);

        playNote(293.665, 750); // D4
        playNote(391.995, 750); // G4
        playNote(329.628, 750); // E4
        playNote(261.626, 750); // C4
        playNote(220, 250); // A3
        playNote(246.942, 250); // B3
        playNote(261.626, 250); // C4
        playNote(293.665, 500); // D4
        playNote(329.628, 250); // E4
        playNote(293.665, 1000); // D4
        playNote(0, 500);

        playNote(329.628, 250); // E4
        playNote(349.228, 250); // F4
        playNote(329.628, 250); // E4
        playNote(293.665, 250); // D4
        playNote(391.995, 500); // G4
        playNote(329.628, 250); // E4
        playNote(293.665, 250); // D4
        playNote(261.626, 1000); // C4
        playNote(0, 250);

        playNote(293.665, 250); // D4
        playNote(329.628, 500); // E4
        playNote(261.626, 250); // C4
        playNote(220, 500); // A3
        playNote(261.626, 250); // C4
        playNote(220, 250); // A3
        playNote(195.998, 1000); // G3
        playNote(0, 500);

        playNote(195.998, 250); // G3
        playNote(261.626, 500); // C4
        playNote(329.628, 250); // E4
        playNote(293.665, 500); // D4
        playNote(195.998, 250); // G3
        playNote(261.626, 500); // C4
        playNote(329.628, 250); // E4
        playNote(293.665, 250); // D4
        playNote(329.628, 250); // E4
        playNote(349.228, 250); // F4
        playNote(391.995, 250); // G4
        playNote(329.628, 250); // E4
        playNote(261.626, 250); // C4
        playNote(293.665, 500); // D4
        playNote(195.998, 250); // G3
        playNote(261.626, 1500); // C4
        playNote(0, 5000);
    }
}

int main()
{
    char bnum=0;
    char bhit=0;
    mux.lock();
    //Change uLCD text to readable size
    uLCD.text_width(8); 
    uLCD.text_height(8);
    mux.unlock();
    //Starts all threads
    Thread uLCDT(uLCD_thread);
    Thread frontsonar(frontsonar_thread);
    Thread backsonar(backsonar_thread);
    Thread speaker(speaker_thread);
    //Main loop is the bluetooth controller thread 
    //Uses Bluetooth to contoller the motors
    while(1) {
        while (!blue.readable()) Thread::yield();
        mux.lock();
        if (blue.getc()=='!') {
            if (blue.getc()=='B') { //button data packet
                bnum = blue.getc(); //button number
                bhit = blue.getc(); //1=hit, 0=release
                if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) { //checksum OK?
                    myled = bnum - '0'; //current button number will appear on LEDs
                    switch (bnum) {
                        //butons 1-4 vhange the current control mode 
                        case '1': //number button 1
                            if (bhit=='1') {
                                mode = 1;
                                s = -0.5;
                            } else {
                                left.coast();
                                right.coast();
                            }
                            break;
                        case '2': //number button 2
                            if (bhit=='1') {
                                mode = 2;
                                s = 0;
                            } else {
                                left.coast();
                                right.coast();
                            }
                            break;
                        case '3': //number button 3
                            if (bhit=='1') {
                                mode = 3;
                                s = -0.5;
                            } else {
                                left.speed(s);
                                right.speed(s);
                            }
                            break;
                        case '4': //number button 4
                            if (bhit=='1') {
                                mode = 4;
                                s = -0.5;
                            } else {
                                left.speed(s);
                                right.speed(s);
                            }
                            break;
                        //Directional arrows do nothing in mode 3 and 4
                        case '5': //button 5 up arrow
                            if (bhit=='1') {
                                if (mode == 1) {
                                    s = -0.5;
                                    left.speed(s);
                                    right.speed(s);
                                }
                            } else {
                                if (mode == 1) {
                                    left.coast();
                                    right.coast();
                                }
                                else if (mode == 2 && s < 1) {
                                    s -= 0.1;
                                    left.speed(s);
                                    right.speed(s);
                                }
                            }
                            break;
                        case '6': //button 6 down arrow
                            if (bhit=='1') {
                                if (mode == 1) {
                                    s = 0.5;
                                    left.speed(s);
                                    right.speed(s);
                                }
                            } else {
                                if (mode == 1) {
                                    s = 0.5;
                                    left.coast();
                                    right.coast();
                                }
                                else if (mode == 2 && s > -1) {
                                    s += 0.1;
                                    left.speed(s);
                                    right.speed(s);
                                }
                            }
                            break;
                        case '7': //button 7 left arrow
                            if (bhit=='1') {
                                if (mode <= 2) {
                                    left.speed(s/2.0);
                                    right.speed(s);
                                }
                            } else {
                                if (mode == 1) {
                                    left.coast();
                                    right.coast();
                                }
                                else if (mode == 2) {
                                    left.speed(s);
                                    right.speed(s);
                                }
                            }
                            break;
                        case '8': //button 8 right arrow
                            if (bhit=='1') {
                                if (mode <= 2) {
                                    left.speed(s);
                                    right.speed(s/2.0);
                                }
                            } else {
                                if (mode == 1) {
                                    left.coast();
                                    right.coast();
                                }
                                else if (mode == 2) {
                                    left.speed(s);
                                    right.speed(s);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        mux.unlock();
    }
}