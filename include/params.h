#ifndef PARAMS_H
#define PARAMS_H

// control pins (ignition and arm pins)
#define ARMING_PIN 2
#define IGNITION_PIN 8

// number of sensors | LS: load cell, PT: pressure transducer, TC: thermocouple
#define NUM_LS 3
#define NUM_PT 1
#define NUM_TC 1

#define LC_MAX_V 3.3
#define TRANS_MAX_V 3.3
#define TC_MAX_V 3.3

// sensor data pin arrays
const int ls_pins[NUM_LS] = {14, 15, 16};
const int pt_pins[NUM_PT] = {10};
const int tc_sdi_pins[NUM_TC] = {26};
const int tc_sdo_pins[NUM_TC] = {1};

const int tc_cs_pins[NUM_TC] = {0};

// sensor clock pin arrays, all 13 for now but can change if needed
const int ls_clk_pins[NUM_LS] = {13, 13, 13};
const int pt_clk_pins[NUM_PT] = {13};
const int tc_clk_pins[NUM_TC] = {27};

// enter actual calibration values as needed
const int ls_calibration_values[NUM_LS] = {100, 100, 100};
const int pt_calibration_values[NUM_PT] = {100};
const int tc_calibration_values[NUM_TC] = {100};

// sensor names
#define LOADCELL "LS"
#define TRANSDUCER "PT"
#define THERMOCOUPLE "TC"

// num of sensor types
#define NUM_SENSOR_TYPES 3

// error codes
#define ERR_ANALOG_OUTPUT -1


// data structure to hold process states


#endif