#include <stdlib.h>
#include <Arduino.h>
#include "functions.h"
// #include "usb_serial.h"
// #include <SD.h>

using namespace std;

String inData, outData;
int pendingCommand = 0;
SampleData *sensorData;
// String args[];


// Teensy listens for control commands from RPi
// Called at the end of loop() when incoming serial data is available
void serialEvent() {
    // read incoming serial data
    inData = Serial.readStringUntil('\n');
    pendingCommand = 1;
}

void setup() {
    Serial.begin(115200);
    // delay(1000);
    while(!Serial); // wait for SerialUSB connection to open

    // dynamically initializes multiple HX711's for load cells
    // all HX711 module amps share the same clock pin
    Serial.println("-------------------------------------------");
    Serial.println("Pin setup beginning...");
    setupPins();
    Serial.println("Pin setup complete!");
    Serial.println("-------------------------------------------");
}

void loop() {
    // runCommand executes appropriate controls when the Pi sends a command
    if(pendingCommand) {
        runCommand(inData);
        pendingCommand = 0;
    }

    readSensorValues(sensorData);
    outData = convertToCSV(sensorData);

    // send measurement readings out to Pi though USB
    Serial.println(outData);

    // serialEvent runs if input data is available
    delay(200);
}




// FIXME: use RTC timestamps for data sent to Pi