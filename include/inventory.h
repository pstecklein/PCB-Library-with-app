#include "../src/chips/controller.h"

// Inventory class that holds data regarding controllers and sensors on the PCB
class Inventory {
public:
    std::vector<controller*> controllers;
    Inventory(int numControllers, int numSensorsPerController);
};