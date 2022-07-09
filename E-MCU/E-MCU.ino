#include "pins.h"
#include "message.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

bool reverseStat = false;

void setup() {
    pinMode(RPMPin, INPUT);
    pinMode(reversePin, INPUT);
    pinMode(rearBrakePin, INPUT);
    pinMode(currentOutPin1, OUTPUT);
    pinMode(currentOutPin2, OUTPUT);
    pinMode(currentOutPin3, OUTPUT);
    lcd.begin(16, 2);
    Serial.begin(115200);
}

void loop() {
    int eGearStat = analogRead(eGearPin);
    // small note, likely will have to do some math with voltVal to get the correct voltage reading
    int refVolt = analogRead(voltPin);
    int current1 = analogRead(currentInPin1);
    int current2 = analogRead(currentInPin2);
    int M_MCU = analogRead(serIn);
    int throttleStat = analogRead(throttlePin);
    int eBrakeStat = analogRead(eBrakePin);

    message msg;

    if (digitalRead(rearBrakePin) == LOW){
        msg.reverse = reverse(digitalRead(reversePin));
        msg.ebrake = eBrake(eBrakeStat);
        msg.throttle = throttle(eBrakeStat, throttleStat);
    }
    else {
      msg.reverse = false;
      msg.ebrake = false;
      msg.throttle = 0;
    }
    Serial.println(msg.reverse);
    Serial.println(msg.ebrake);
    Serial.println(int(msg.throttle));

    lcd.print("RPM:");
    lcd.setCursor(4, 0);
    lcd.print(RPMSense(digitalRead(RPMPin)));
    lcd.setCursor(9, 0);
    lcd.print("TT:");
    lcd.setCursor(12, 0);
    lcd.print(throttleStat);
    lcd.setCursor(0, 1);
    lcd.print("Rtr:");
    lcd.setCursor(4, 1);
    lcd.print(1234);
    lcd.setCursor(9, 1);
    lcd.print("BV:");
    lcd.setCursor(12, 1);
    lcd.print(1234);
    Serial.println(" ");
    delay(500);
}

void rotor() {
    // runs the rotor
}

int RPMSense(int value) {
    // display the RPM of the motor
}

void eGear(int val) {
    // information about the volts it should run on rotor
}

void current() {
    // measures the amount of current from the 6 phase drivers & rotor drivers
}

bool reverse(int val) {
    bool status = false;
    if (val == LOW) {
        status = false;
    }
    else {
        status = true;
    }
    return status;
}

bool eBrake(int val) {
    bool status = false;
    if (val == LOW) {
        status = false;
    }
    else {
        status = true;
    }
    return status;
}

char throttle(int eb, int val) {
    if (eb == 0) {
        val = val;
    } 
    else {
        val = 0;
    }
    return val;
}