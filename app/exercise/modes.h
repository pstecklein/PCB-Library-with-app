#include "../../include/inventory.h"
#include "../../include/data_collection.h" // Methods for collecting data from chips on the PCB

// Use this method to inject the hardcoded unique id values into the inventory object
void hardcodeUniqueIds(Inventory *inv);

// Convert raw temp data into readable celcius value
double convertTemp(uint t1, uint t2);

// Take inventory of the unique IDs of all controllers and sensors on the PCB, and save to a file
void takeInventory(Inventory *inv);

// Read sensor (temperature) data and either store it to a file
void readAllTemperatureSensorData();
