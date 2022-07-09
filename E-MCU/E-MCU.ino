#include "pins.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

bool reverseStat = false;

void setup() {
    pinMode(RPMPin, INPUT);
    pinMode(reversePin, INPUT);
    pinMode(currentOutPin1, OUTPUT);
    pinMode(currentOutPin2, OUTPUT);
    pinMode(currentOutPin3, OUTPUT);
    lcd.begin(16, 2);
    Serial.begin(9600);
}

void loop() {
    int eGearStat = analogRead(eGearPin);
    // small note, likely will have to do some math with voltVal to get the correct voltage reading
    int voltVal = analogRead(voltPin);
    int current1 = analogRead(currentInPin1);
    int current2 = analogRead(currentInPin2);
    int M_MCU = analogRead(serIn);
    int throttleStat = analogRead(throttlePin);
    int eBrakeStat = analogRead(eBrakePin);
    int rearBrakeStat = analogRead(rearBrakePin);
    lcd.print("RPM: ");
    lcd.setCursor(5, 0);
    int RPM = RPMSense(digitalRead(RPMPin));
    lcd.print(RPM);
    lcd.setCursor(0, 1);
    lcd.print("TT: ");
    lcd.setCursor(0, 0);
    lcd.print(throttleStat);
}

void rotor() {

}

int RPMSense(int value) {

}

void eGear() {

}

void current() {

}

void reverse() {

}