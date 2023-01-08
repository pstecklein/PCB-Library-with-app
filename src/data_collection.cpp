#include <iostream>
#include <vector>

#include "../include/data_collection.h"
#include "../drivers/bus.h" // Drivers contain our "mock" functions (write/read)


uint readSensor(std::byte cBusId, std::byte sBusId, sensorPortions &portions, uint address) {
    // Load controller(cBusId) SENSORBUS_BUSID with sBusId
    write(cBusId, portions.busId, (uint)sBusId);

    // Load controller(cBusId) SENSORBUS_ADDR with address
    write(cBusId, portions.addr, address);

    // Load controller(cBusId) SENSORBUS_OP with 0
    write(cBusId, portions.op, 0);

    /*** Read controller(cBusId) SENSORBUS_STATUS[0] ***/
    // Wait until SENSORBUS_STATUS[0] == 1
    // while(!(read(cBusId, portions.status) & 1)) {
    //     printf("Waiting for SensorBus read operation");
    // }
    // Wait until SENSORBUS_STATUS[0] == 0
    // while(read(cBusId, portions.status) & 1) {
    //     printf("reading sensor %d from controller %d \n", sBusId, cBusId);
    // }

    // Read controller(cBusId) SENSORBUS_DATA
    read(cBusId, portions.data);
    return rand() % 255; // Return hardcoded value for this exercise
}

uint readController(std::byte cBusId, uint address) {
    return read(cBusId, address);
}

std::vector<int> readSensorDataPerController(std::byte cBusId, std::vector<int> sensors, sensorPortions &portions, uint address) {
    std::vector<int> res;

    // Read the data from each sensor
    for (int i : sensors) {
        res.push_back(readSensor(cBusId, (std::byte)i, portions, address));
    }
    return res;
}

std::vector<int> readMultipleControllersSensorData(std::vector<int> controllers, std::vector<int> sensors, sensorPortions &portions, uint address) {
    std::vector<int> res;

    // Read the data from each sensor for each controller
    std::vector<int> selectedSensorData;
    for (int i : controllers) {
        selectedSensorData = readSensorDataPerController((std::byte)i, sensors, portions, address);
        for (int data : selectedSensorData) {
            res.push_back(data);
        }
    }
    return res;
}