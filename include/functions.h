#pragma once

#include "params.h"
#include "Arduino.h"
#include "structs.h"

float readLoadCell0() {
    int rawVal = analogRead(LOADCELL0); // read raw data from load cell
    float normalizedVal = LC_MAX_V * (rawVal / 1024); // convert from raw data to voltage units
    return normalizedVal;
}

float readLoadCell1() {
    int rawVal = analogRead(LOADCELL1); // read raw data from load cell
    float normalizedVal = LC_MAX_V * (rawVal / 1024); // convert from raw data to voltage units
    return normalizedVal;
}

float readTransducer() {
    int rawVal = analogRead(TRANSDUCER);
    float normalizedVal = TRANS_MAX_V * (rawVal / 1024);
    return normalizedVal;
}

float readThermocouple() {
    int rawVal = analogRead(TRANSDUCER);
    float normalizedVal = TC_MAX_V * (rawVal / 1024);
    return normalizedVal;
}

void ignitionOn() {
    digitalWrite(IGNITION_RELAY, HIGH);
    Serial.println("Ignition on!");
}

void ignitionOff() {
    digitalWrite(IGNITION_RELAY, LOW);
    Serial.println("Ignition off!");
}

void armCircuit() {
    digitalWrite(ARM_RELAY, HIGH);
    Serial.println("Armed circuit!");
}

void disarmCircuit() {
    digitalWrite(ARM_RELAY, LOW);
    Serial.println("Disarmed circuit!");
}

void setupPins() {
    pinMode(IGNITION_RELAY, OUTPUT);
    pinMode(ARM_RELAY, OUTPUT);
    pinMode(LOADCELL0, INPUT);
    pinMode(LOADCELL1, INPUT);
    pinMode(TRANSDUCER, INPUT);
    pinMode(THERMOCOUPLE, INPUT);
}

void runCommand(String command) {
    if(command == "ignition on") {
        ignitionOn();
    }
    else if (command == "ignition off") {
        ignitionOff();
    }
    else if (command == "arm on") {
        armCircuit();
    }
    else if (command == "arm off") {
        disarmCircuit();
    }
}

SampleData readSensorValues() {
    float lcVal, tVal, tcVal; // load cell, transducer, thermocouple values
    lcVal = readLoadCell0();
    tVal = readTransducer();
    tcVal = readThermocouple();
    
    return {lcVal, tVal, tcVal};
}