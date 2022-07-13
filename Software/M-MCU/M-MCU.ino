// Motor MCU Code (ESP32)
// Dallas Hart
// Summer 2022

#include "pindefs.h"
#include "motor_controller.h"
#include "../message.h"

#define BAUD_RATE 115200

void setup() {
    Serial.begin(BAUD_RATE);

    // Wait for valid connection
    while(Serial.available() == 0){ 
        MotorController::beep(1000, 100);
        delay(2000); 
    }

    if(Serial.readStringUntil('\n') != "Req to connect to M-MCU"){ // Requires hard reset
        while(true){ 
            MotorController::beep(1000, 100);
            delay(100);
        } 
    }

    Serial.write("M-MCU ack\n");

    MotorController::init(); // initialize initial hall sensor positions 
}

void loop() {
    char bytes_available = Serial.available();
    if (bytes_available == sizeof(message)) {
        byte buf[sizeof(message)];
        Serial.readBytes(buf, sizeof(message));
        message recv = { (bool)buf[0], (bool)buf[1], (char)buf[2] };
        MotorController::update(recv);
    }
}