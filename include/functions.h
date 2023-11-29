#pragma once

#include "params.h"
#include "Arduino.h"
#include "structs.h"
#include <Adafruit_MAX31856.h>
#include "HX711.h"
#include <vector>
#include <Wire.h>

vector<HX711> scalesLS;
vector<HX711> scalesPT;
vector<Adafruit_MAX31856> ampsTC;

float readLoadCell(HX711 scale) {
    float reading = scale.get_units();
    return reading;
}

float readTransducer(HX711 scale) {
    float reading = scale.get_units();
    return reading;
}

float readThermocouple(Adafruit_MAX31856 amp) {
    float reading = amp.readThermocoupleTemperature();
    return reading;
}

void ignitionOn() {
    digitalWrite(IGNITION_PIN, HIGH);
    Serial.println("Ignition on!");
}

void ignitionOff() {
    digitalWrite(IGNITION_PIN, LOW);
    Serial.println("Ignition off!");
}

void armCircuit() {
    digitalWrite(ARMING_PIN, HIGH);
    Serial.println("Armed ignition!");
}

void disarmCircuit() {
    digitalWrite(ARMING_PIN, LOW);
    Serial.println("Disarmed ignition!");
}

void setupPins() {
    pinMode(IGNITION_PIN, OUTPUT);
    pinMode(ARMING_PIN, OUTPUT);

    Serial.println("Setting up HX711 load cell amps.");
    for(int i = 0; i < NUM_LS; i++) {
        HX711 scale;
        scale.begin(ls_pins[i], ls_clk_pins[i]);
        scale.set_scale(ls_calibration_values[i]);
        scale.tare();
        scalesLS.push_back(scale);
    }
    Serial.println("HX711 load cell amps are good to go!");

    // dynamic initialization of HX711's for pressure transducers
    Serial.println("Setting up HX711 pressure transducer amps.");
    for(int i = 0; i < NUM_PT; i++) {
        HX711 scale;
        scale.begin(pt_pins[i], pt_clk_pins[i]);
        scale.set_scale(pt_calibration_values[i]);
        scale.tare();
        scalesPT.push_back(scale);
    }
    Serial.println("HX711 pressure transducer amps are good to go!");

    // dynamic initialization of thermocouple amps
    Serial.println("Setting up thermocouple amps.");
    for(int i = 0; i < NUM_TC; i++) {
        // using hardware spi, passing cs pin
        Adafruit_MAX31856 max = Adafruit_MAX31856(tc_cs_pins[i]);
        max.begin();
        max.setThermocoupleType(MAX31856_TCTYPE_K);
        ampsTC.push_back(max);
    }
    Serial.println("Thermocouple amps are good to go!");
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

void readSensorValues(SampleData *data) {

    // read LC values
    for(int i = 0; i < NUM_LS; i++) {
        data->loadcellValues.at(i) = readLoadCell(scalesLS.at(i));
    }

    // read PT values
    for(int i = 0; i < NUM_PT; i++) {
        data->transducerValues.at(i) = readTransducer(scalesPT.at(i));
    }

    // read TC values
    for(int i = 0; i < NUM_TC; i++) {
        data->thermocoupleValues.at(i) = readThermocouple(ampsTC.at(i));
    }
}

String convertToCSV(SampleData *data) {
    String csvData = "";
    for(int i = 0; i < NUM_LS; i++) {
        csvData.append(String(data->loadcellValues.at(i)));
        csvData.append(",");
    }

    for(int i = 0; i < NUM_PT; i++) {
        csvData.append(String(data->transducerValues.at(i)));
        csvData.append(",");
    }

    for(int i = 0; i < NUM_TC; i++) {
        csvData.append(String(data->thermocoupleValues.at(i)));
        csvData.append(",");
    }

    // remove last comma
    csvData.remove(csvData.length() - 1);
}