#include <vector>

#include "sensor.h"

// Controller struct - holds address data and sensor structs
struct controller {
    std::unordered_map<int, int> addresses;
    std::vector<sensor*> sensors; // sensor slaves
    int addr_sel_pins; // address-select pins to configure unique "Bus ID"
    controller(int pins, int numSensors);
};