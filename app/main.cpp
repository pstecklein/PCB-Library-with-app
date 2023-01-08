#include <fstream>
#include <array>

#include "constants/constants.h"
#include "exercise/modes.h"


int main()
{

    // initialize inventory
    Inventory *inv;
    inv = new Inventory(NUMCONTROLLERS, SENSORSPERCONTROLLER);

    // inject hardcoded values to be the unique IDs of the chips on the PCB
    hardcodeUniqueIds(inv);

    // Take inventory of the unique IDs of all controllers and sensors on the PCB, and save to a file
    takeInventory(inv);

    // For this exercise, just read temperature data once
    // Of course in a real setting we might want to read continuously
    readAllTemperatureSensorData();

    printf("Complete! Check txt files for output\n");
    return 0;
}