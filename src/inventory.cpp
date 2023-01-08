#include "../include/inventory.h"


Inventory::Inventory(int numControllers, int numSensorsPerController) {
    // initialize the inventory
    for (int i = 0; i < numControllers; i++) {
        controllers.push_back(new controller(i, numSensorsPerController));
    }
}