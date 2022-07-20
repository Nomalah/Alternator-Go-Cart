#include "pins.h"
#include "../message.h"
#include <LiquidCrystal.h>
#include <ESP32Servo.h>

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Servo servo;

#define EBRAKE_THRESHOLD 0
#define HALL_THRESHOLD 100

bool next = false;
long times;
long prevTime;
long lastTime;
int taken;

byte readAnalogESP(int pin) {
    return map(analogRead(pin), 0, 4095, 0, 255);
}

void RPMISR() {
    times = micros();
    next = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(RPMPin, INPUT);
    pinMode(reversePin, INPUT);
    pinMode(rearBrakePin, INPUT);
    pinMode(currentOutPins[0], OUTPUT);
    pinMode(currentOutPins[1], OUTPUT);
    pinMode(currentOutPins[2], OUTPUT);
    pinMode(rotorPinPWM, OUTPUT);
    pinMode(rotorPinEN, OUTPUT);
    // pinMode(lights, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(RPMPin), RPMISR, FALLING);
    lcd.begin(16, 2);
    servo.attach(9);
    servo.writeMicroseconds(1500);
    delay(1000);
}

void loop() {
    // section 1 - pin setups - no need to move/cannot move
    int eGearStat = analogRead(eGearPin); // may not be used, but is placeholder
    float refVolt = ((analogRead(voltPin) * 3.3)/4096) * 13;
    int throttleStat = readAnalogESP(throttlePin);
    int eBrakeStat = readAnalogESP(eBrakePin);
    Serial.println("Reference voltage: ");
    Serial.print(refVolt);
    Serial.print("V");

    // section 2 - M-MCU comms - moved
    mmcu(eBrakeStat, throttleStat);

    // section 3 - current sensing - in progress, no move for now
    Serial.println(current());

    // section 4 - run the rotor - already moved
    int rV = map(eGearStat, 0, 1023, 21, 149); // placeholder until eGear code figured out
    rotor(rV, throttleStat);

    // section 5 - lcd display driver
    display(throttleStat, refVolt, rV);
}

// section 2
void mmcu(int eBrakeStat, int throttleStat) {
    message msg;

    // digitalWrite(lights, LOW);

    if (digitalRead(rearBrakePin) == HIGH) {
        Serial.println("Rear brake engaged");
        msg.reverse = false;
        msg.ebrake = false;
        msg.throttle = 0;
        // digitalWrite(lights, HIGH);
    } 
    else if (eBrakeStat > EBRAKE_THRESHOLD) {
        Serial.println("E-Brake Engaged");
        msg.reverse = false;
        msg.ebrake = true;
        msg.throttle = eBrakeStat;
    } 
    else {
        Serial.println("Normal operation");
        msg.reverse = digitalRead(reversePin) == HIGH;
        msg.ebrake = false;
        msg.throttle = throttleStat;
    }

    Serial.println(msg.reverse);
    Serial.println(msg.ebrake);
    Serial.println(int(msg.throttle));

    Serial.write((byte*)(&msg), sizeof(message));
}

// section 5
void display(int throttleStat, int refVolt, int rV){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RPM:");
    lcd.setCursor(9, 0);
    lcd.print("TT:");
    lcd.setCursor(0, 1);
    lcd.print("BV:");
    lcd.setCursor(9, 1);
    lcd.print("RtV:");
    lcd.setCursor(12, 0);
    lcd.print(throttleStat);
    lcd.setCursor(4, 1);
    lcd.print(refVolt);
    lcd.print("V");
    lcd.setCursor(12, 1);
    lcd.print(rV);
    lcd.setCursor(4, 0);
    RPM(); // RPM lcd print is in this function
    Serial.println();
}

void RPM() {
    if (next == true) {
        prevTime = lastTime;
        noInterrupts();
        lastTime = times;
        next = false;
        interrupts();
        taken = lastTime - prevTime - 100000;
        int val = 30000000/taken;
        Serial.println(taken);
        Serial.println();
        Serial.println(val);
        lcd.print(val);
        delay(100);
    }
}

// don't know if the code below will work; something seems off about it
int current() {
    int current1 = analogRead(currentInPin1);
    int current2 = analogRead(currentInPin2);
    Serial.println("Current values:");
    for (byte pin=0; pin<=7; pin++) {
        multiplexerCycle(pin);
        Serial.println(pin + 1);
        int inputValue = current1;
        Serial.print("-" + String(inputValue));
    }
}

void multiplexerCycle(byte pin)
{
    for (int i=0; i<3; i++){
        if (pin & (1<<i))
            digitalWrite(currentOutPins[i], HIGH);
        else
            digitalWrite(currentOutPins[i], LOW);
    }
}

// incomplete code below
void rotor(int val, int throttle) {
    analogWrite(rotorPinPWM, val);
    digitalWrite(rotorPinEN, throttle > 250);
    servo.writeMicroseconds(throttle);
    /* code from original go kart
    analogWrite(11, actualGear); // this was rotor pwm
    digitalWrite(6, outputThrottle > 1490);
    servo.writeMicroseconds(outputThrottle);*/
}

int eGear() {
    // calculates the volts the rotor should run
    int val;
    return val;
}