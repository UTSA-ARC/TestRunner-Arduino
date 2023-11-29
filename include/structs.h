#include <array>
#include "params.h"

using namespace std;

struct SampleData {
    array<float, NUM_LS> loadcellValues;
    array<float, NUM_PT> transducerValues;
    array<float, NUM_TC> thermocoupleValues;
};

// struct LS {
//     int dataPin;
//     int clkPin;
// };