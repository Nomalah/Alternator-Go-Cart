// Motor MCU Code (ESP32)
// Dallas Hart
// Summer 2022

#include "pindefs.h"
#include "../message.h"

#define BAUD_RATE 115200

class MotorController {
    MotorController() {
        // Attach interupts?
    }
    void update(message new_values) {
        
    }
}

MotorController phaseController();

void setup() {
    Serial.begin(BAUD_RATE);


    Serial.write("Beginning control loop...")
}

void loop() {
    char bytes_available = Serial.available();
    if (bytes_available == 0) {
        
    } else if (bytes_available == sizeof(message)) {
        byte buf[sizeof(message)];
        Serial.readBytes(buf, sizeof(message));
        message recv = { (bool)buf[0], (bool)buf[1], (char)buf[2] };
        MotorController.update(recv);
    }
}