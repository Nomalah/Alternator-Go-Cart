#include "pins.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(38, 36, 35, 27, 25, 21);

void setup() {
    pinMode(RPMPin, INPUT);
    pinMode(currentOutPin1, OUTPUT);
    pinMode(currentOutPin2, OUTPUT);
    pinMode(currentOutPin3, OUTPUT);
    lcd.begin(16, 2);
}

void loop() {
    int eGearStat = analogRead(eGearPin);
    // small note, likely will have to do some math with voltVal to get the correct voltage reading
    int voltVal = analogRead(voltPin);
    int current1 = analogRead(currentInPin1);
    int current2 = analogRead(currentInPin2);
    int M_MCU = analogRead(serIn);
    // int throttleStat = analogRead(throttlePin);
    // int eBrakeStat = analogRead(eBrakePin);
    // int rearBrakeStat = analogRead(rearBrakePin);
}

void rotor() {

}

void RPMSense() {

}

void eGearSend() {

}

void current() {

}
