// data collection methods - we could of course extend this library by adding new functions

#include "../src/portionStructures/sensorPortions.h"

// Read data from a single sensor
uint readSensor(std::byte cBusId, std::byte sBusId, sensorPortions &portions, uint address);

// Read data from a single controller
uint readController(std::byte cBusId, uint address);

// Read data from multiple sensors for a specific controller
std::vector<int> readSensorDataPerController(std::byte cBusId, std::vector<int> sensors, sensorPortions &portions, uint address);

// Read data from multiple sensors for multiple controllers
std::vector<int> readMultipleControllersSensorData(std::vector<int> controllers, std::vector<int> sensors, sensorPortions &portions, uint address);