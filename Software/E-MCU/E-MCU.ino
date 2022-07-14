#include "pins.h"
#include "../message.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

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
    pinMode(RPMPin, INPUT);
    pinMode(reversePin, INPUT);
    pinMode(rearBrakePin, INPUT);
    pinMode(currentOutPins[0], OUTPUT);
    pinMode(currentOutPins[1], OUTPUT);
    pinMode(currentOutPins[2], OUTPUT);
    pinMode(rotorPin1, OUTPUT);
    pinMode(rotorPin2, OUTPUT);
    // pinMode(lights, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(RPMPin), RPMISR, FALLING);
    lcd.begin(16, 2);
    Serial.begin(115200);
}

void loop() {
    // int eGearStat = analogRead(eGearPin); --> may not be used
    // small note, likely will have to do some math with refVolt to get the correct voltage reading
    float refVolt = ((analogRead(voltPin) * 3.3)/4096) * 13;
    int throttleStat = readAnalogESP(throttlePin);
    int eBrakeStat = readAnalogESP(eBrakePin);

    Serial.println("Reference voltage: ");
    Serial.print(refVolt);

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

    // Send msg to M-MCU
    Serial.write((byte*)(&msg), sizeof(message));

    // Current sensing
    Serial.println(current());

    // Print info on LCD
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
    lcd.print(1234); // placeholder
    lcd.setCursor(4, 0);
    RPM();
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
void rotor() {
    int voltage = eGear();
    
}

int eGear() {
    // calculates the volts the rotor should run
    int val;
    return val;
}