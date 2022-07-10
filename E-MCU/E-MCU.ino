#include "pins.h"
#include "../message.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

#define EBRAKE_THRESHOLD 0
#define HALL_THRESHOLD 100.0

byte readAnalogESP(int pin){
    return map(analogRead(pin), 0, 4095, 0, 255);
}

void setup() {
    pinMode(RPMPin, INPUT);
    pinMode(reversePin, INPUT);
    pinMode(rearBrakePin, INPUT);
    pinMode(currentOutPin1, OUTPUT);
    pinMode(currentOutPin2, OUTPUT);
    pinMode(currentOutPin3, OUTPUT);
    pinMode(rotorPin1, OUTPUT);
    pinMode(rotorPin2, OUTPUT);
    // pinMode(lights, OUTPUT);
    lcd.begin(16, 2);
    Serial.begin(115200);
}

void loop() {
    int eGearStat = analogRead(eGearPin);
    // small note, likely will have to do some math with refVolt to get the correct voltage reading
    int refVolt = analogRead(voltPin);
    int current1 = analogRead(currentInPin1);
    int current2 = analogRead(currentInPin2);
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

    // Print info on LCD Display
    lcd.clear();
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
    lcd.print(refVolt);
    Serial.println(" ");
    delay(500);
}

void rotor() {
    // runs the rotor
}

int RPMSense(int val) {
    float hall_count = 1.0;
    float start = micros();
    bool on_state = false;
    while(true){
        if (val == LOW){
            if (on_state==false){
                on_state = true;
                hall_count+=1.0;
            }
        } 
        else{
            on_state = false;
        }
        if (hall_count>=HALL_THRESHOLD){
            break;
        }
    }
    float end_time = micros();
    float time_passed = ((end_time-start)/1000000.0);
    float rpm_val = (hall_count/time_passed)*60.0;
    int RPM = int(rpm_val);
    return RPM;
}

void eGear(int val) {
    // information about the volts it should run on rotor
}

void current() {
    // measures the amount of current from the 6 phase drivers & rotor drivers
}

bool reverse(int val) {
    return val == HIGH;
}

bool eBrake(int val) {
    return val == HIGH;
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