#include <stdlib.h>
#include <Arduino.h>
#include "functions.h"
// #include "usb_serial.h"
// #include <SD.h>

String inData, outData;
int pendingCommand = 0;
SampleData sensorData;



// Nano listens for control commands from RPi
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


    setupPins();
}

void loop() {
    // runCommand executes appropriate controls when the Pi sends a command
    if(pendingCommand) {
        runCommand(inData);
        pendingCommand = 0;
    }

    sensorData = readSensorValues();

    // dataOut = String(loadCellData) + "," + String(transducerData) + "," + String(thermocoupleData);
    outData = String(sensorData.loadcellVal) + "," + String(sensorData.transducerVal) + "," + String(sensorData.thermocoupleVal);

    // send measurement readings out to Pi though USB
    Serial.println(outData);
}

