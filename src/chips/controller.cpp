#include "controller.h"


controller::controller(int pins, int numSensors) {
    addr_sel_pins = pins;
    for (int i = 0; i < numSensors; i++) {
        sensors.push_back(new sensor(i));
    }
}