#include "pins.h"
#include "../message.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

#define EBRAKE_THRESHOLD 0
#define HALL_THRESHOLD 100

bool next = false;
long time;
long prevTime;
long lastTime;
int taken;

byte readAnalogESP(int pin) {
    return map(analogRead(pin), 0, 4095, 0, 255);
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
    // something like int refVolt = (analogRead(voltPin) * x)/4096
    int refVolt = analogRead(voltPin);
    int throttleStat = readAnalogESP(throttlePin);
    int eBrakeStat = readAnalogESP(eBrakePin);

    message msg;

    // digitalWrite(lights, LOW);

    if (digitalRead(rearBrakePin) == HIGH) {
        Serial.println("Rear brake engaged");
        msg.reverse = false;
        msg.ebrake = false;
        msg.throttle = 0;
        // digitalWrite(lights, HIGH);
    } else if (eBrakeStat > EBRAKE_THRESHOLD) {
        Serial.println("E-Brake Engaged");
        msg.reverse = false;
        msg.ebrake = true;
        msg.throttle = eBrakeStat;
    } else {
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
    lcd.print("RtV:");

    lcd.setCursor(9, 1);
    lcd.print("BV:");

    lcd.setCursor(12, 0);
    lcd.print(throttleStat);

    lcd.setCursor(4, 1);
    lcd.print(1234); // placeholder

    lcd.setCursor(12, 1);
    lcd.print(12); // placeholder
    lcd.print("V");

    lcd.setCursor(4, 0);
    RPM();
    RPMISR();

    Serial.println();
}

void RPM() {
    if (next == true) {
        prevTime = lastTime;
        noInterrupts();
        lastTime = time;
        next = false;
        interrupts();
        taken = lastTime - prevTime;
        int val = 3000000/taken;
        Serial.println(val);
        lcd.print(val);
    }
}

void RPMISR() {
    time = micros();
    next = true;
}

// old rpm stuff below
int RPMSense() {
    int hall_count = 1;
    int start = micros();
    bool on_state = false;
    while(true){
        if (digitalRead(RPMPin) == LOW){
            if (on_state==false){
                on_state = true;
                hall_count+=1;
            }
        } 
        else{
            on_state = false;
        }
        if (hall_count>=HALL_THRESHOLD){
            break;
        }
    }
    int end_time = micros();
    float time_passed = ((end_time-start)/1000000.0);
    float rpm_val = (hall_count/time_passed)*60.0;
    int RPM = int(rpm_val);
    Serial.println("RPM:");
    Serial.println(RPM);
    return RPM;
}

// don't know if the code below will work; something seems off about it
int current() {
    int current1 = analogRead(currentInPin1);
    int current2 = analogRead(currentInPin2);
    Serial.println("Current values:");
    for (byte pin=0; pin<=7; pin++) {
        selectMuxPin(pin);
        Serial.println(String(pin + 1) + ". ");
        int inputValue = current1;
        Serial.print(inputValue);
    }
}

void selectMuxPin(byte pin)
{
    for (int i=0; i<3; i++){
        if (pin & (1<<i))
            digitalWrite(currentOutPins[i], HIGH);
        else
            digitalWrite(currentOutPins[i], LOW);
    }
}

void rotor() {
    // runs the rotor
}

void eGear(int val) {
    // information about the volts it should run on rotor
}