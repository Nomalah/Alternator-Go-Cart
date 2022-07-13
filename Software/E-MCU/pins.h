#pragma once

const int eGearPin = 33;
const int RPMPin = 32;
const int voltPin = 18;

// Motor related pins
const int reversePin = 34;
const int throttlePin = 36;
const int eBrakePin = 39;
const int rearBrakePin = 35;

// LCD Pins
#define LCD_RS 19
#define LCD_EN 23
#define LCD_D4 22   
#define LCD_D5 21
#define LCD_D6 18
#define LCD_D7 4

// placeholder pins
const int currentInPin1 = 34;
const int currentInPin2 = 35;
const int currentOutPins[3] = {2, 4, 5};
const int rotorPin1 = 25;
const int rotorPin2 = 26;

// in case want brake lights; only use if have enough pins left
//const int lights = 4;