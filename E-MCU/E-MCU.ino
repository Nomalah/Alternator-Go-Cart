#include "pins.h"
#include "LiquidCrystal.h"
int eGearStat;
int voltVal;
int current1;
int current2;
int M_MCU;
// int throttleStat;
// int eBrakeStat;
// int rearBrakeStat;

void setup() {
    pinMode(RPMPin, INPUT);
    pinMode(currentOutPin1, OUTPUT);
    pinMode(currentOutPin2, OUTPUT);
    pinMode(currentOutPin3, OUTPUT);
}

void loop() {
    eGearStat = analogRead(eGearPin);
    voltVal = analogRead(voltPin);
    current1 = analogRead(currentInPin1);
    current2 = analogRead(currentInPin2);
    M_MCU = analogRead(serIn);
    // throttleStat = analogRead(throttlePin);
    // eBrakeStat = analogRead(eBrakePin);
    // rearBrakeStat = analogRead(rearBrakePin);
}