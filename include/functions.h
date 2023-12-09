#pragma once


//fixes: removed vector, created HX711 arrays instead
// added some commands

#include "params.h"
#include "Arduino.h"
#include "structs.h"
#include <Adafruit_MAX31856.h>
#include "HX711.h"
#include <Wire.h>

HX711 scalesLS[NUM_LS];
HX711 scalesPT[NUM_PT];
// Adafruit_MAX31856 ampsTC[NUM_TC];

float readLoadCell(HX711 *scale) {
    float reading = scale->get_units(10);
    return reading;
}

float readTransducer(HX711 *scale) {
    float reading = scale->get_units(10);
    return reading;
}

float readThermocouple(Adafruit_MAX31856 *amp) {
    float reading = amp->readThermocoupleTemperature();
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
        scalesLS[i] = scale;
    }
    Serial.println("HX711 load cell amps are good to go!");

    // dynamic initialization of HX711's for pressure transducers
    Serial.println("Setting up HX711 pressure transducer amps.");
    for(int i = 0; i < NUM_PT; i++) {
        HX711 scale;
        scale.begin(pt_pins[i], pt_clk_pins[i]);
        scale.set_scale(pt_calibration_values[i]);
        scale.tare();
        scalesPT[i] = scale;
    }
    Serial.println("HX711 pressure transducer amps are good to go!");

    // dynamic initialization of thermocouple amps
    // Serial.println("Setting up thermocouple amps.");
    // for(int i = 0; i < NUM_TC; i++) {
    //     // using hardware spi, passing cs pin
    //     Adafruit_MAX31856 max = Adafruit_MAX31856(tc_cs_pins[i]);
    //     max.begin();
    //     max.setThermocoupleType(MAX31856_TCTYPE_K);
    //     ampsTC[i] = max;
    // }
    // Serial.println("Thermocouple amps are good to go!");
}


// FIXME: add commands: startCollection, stopCollection, checkHealth, pin <pin> <type> <state> [options]
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

// starts DAQ, [options] : -ex <sensor> (excludes sensor)
void startDataCollection(bool *collect_data, int *sensorsDAQ[], String *args[]) {

    *collect_data = true;

    // exclude a sensor if user specified option "-ex"
    if(*args[0] == "-ex") {

        if(*args[1] == LOADCELL) {

            *sensorsDAQ[0] = 0;

        }
        else if(*args[1] == TRANSDUCER) {

            *sensorsDAQ[1] = 0;

        }
        else if(*args[1] == THERMOCOUPLE) {

            *sensorsDAQ[2] = 0;

        }
    }
}

void stopDataCollection(bool *collect_data, int *sensorsDAQ[]) {

    *collect_data = false;

    // reset all sensors to be active if user starts DAQ in the future
    for(int i = 0; i < NUM_SENSOR_TYPES; i++) {

        *sensorsDAQ[i] = 1;

    }
}

int drivePin(int pin, String *type, int state, String *option = NULL) {

    // set pin I/O mode
    if(*type == "input") {

        pinMode(pin, INPUT);

        // return digital pin input
        if(option == NULL) {

            return digitalRead(pin);

        }
        // return analog input value
        else if(*option = "-a") {

            return analogRead(pin);

        }

    }
    else if(*type == "output") {

        pinMode(pin, OUTPUT);
        digitalWrite(pin, state);

        // no analog outputs
        if(*option == "-a") {
            
            Serial.println("Analog output is invalid");
            return ERR_ANALOG_OUTPUT;

        }

    }
    else {
        
        // error: type was not correctly specified
        return -1;

    }

    // FIXME: add analog pin option


    return pin;
}

/*

checkHealth : CheckAmount | [Sub Check ErrorCodes] | ErrorCode - Checks if in normal operation

options:

    -ex [<check>] - Exclude sub check [<sensor>, power, piCont, igCont, sdCard]

*/

// FIXME: find a way to verify the state of all processes. Maybe implement a helper function for each process??
void checkHealth(String *option = NULL, String *process[] = NULL, int num_components = 0) {

    // number of process that will be checked
    int num_checks = NUM_LS + NUM_PT + NUM_TC - num_components;

    // create check amount string and send out to Pi
    char *check_amount;
    sprintf(check_amount, "CheckAmount:%d", num_checks);

    Serial.println(check_amount);

    // Pi will now wait for num_checks checks
    


}

// char *checkLS(HX711 *scale) {
    
// }

void readSensorValues(SampleData *data) {

    // read LC values
    for(int i = 0; i < NUM_LS; i++) {
        data->loadcellValues.at(i) = readLoadCell(&scalesLS[i]);
    }

    // read PT values
    for(int i = 0; i < NUM_PT; i++) {
        data->transducerValues.at(i) = readTransducer(&scalesPT[i]);
    }

    // read TC values
    // for(int i = 0; i < NUM_TC; i++) {
    //     data->thermocoupleValues.at(i) = readThermocouple(&ampsTC[i]);
    // }
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

    return csvData;
}