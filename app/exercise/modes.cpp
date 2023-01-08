#include <fstream>
#include <array>

#include "modes.h"
#include "../constants/constants.h"


/*
specify controllers and sensors we want to read data from
in our case, this is all controllers and sensors on PCB
*/
std::vector<int> getAllSensors() {
    std::vector<int> sensors;
    for (int i = 0; i < SENSORSPERCONTROLLER; i++) sensors.push_back(i);
    return sensors;
}
std::vector<int> getAllControllers() {
    std::vector<int> controllers;
    for (int i = 0; i < NUMCONTROLLERS; i++) controllers.push_back(i);
    return controllers;
}

// relevant portions of the controller
sensorPortions getPortions() {
    struct sensorPortions portions;
    portions.busId = SENSORBUS_BUSID;
    portions.addr = SENSORBUS_ADDR;
    portions.data = SENSORBUS_DATA;
    portions.op = SENSORBUS_OP;
    portions.status = SENSORBUS_STATUS;
    
    return portions;
}


void hardcodeUniqueIds(Inventory *inv) {

    /*
    For this exercise, I decided to initialize inventory with hardcoded values from test/unique_ids.tx
    Read hardcoded hex values from txt file (hardcoded unique IDs)
    loop through controllers, for each controller assign UNIQUE_ID1, UNIQUE_ID2
    For each sensor, assign UNIQUE_ID1, UNIQUE_ID2, UNIQUE_ID3, UNIQUE_ID4
    */

    std::array<int, NUMCONTROLLERS * 2> ControllerUniqueIds;
    std::array<int, NUMCONTROLLERS * SENSORSPERCONTROLLER> SensorUniqueIds;

    std::ifstream idFile("app/test/unique_ids.txt");
    std::string hex;
    long hexLong;
    for (int i = 0; i < (NUMCONTROLLERS * 2); i++) {
        getline(idFile, hex);
        hexLong = std::stol (hex, nullptr, 0);
        ControllerUniqueIds.at(i) = (int)hexLong;
    }
    for (int i = 0; i < (NUMCONTROLLERS * SENSORSPERCONTROLLER); i++) {
        getline(idFile, hex);
        hexLong = std::stol (hex, nullptr, 0);
        SensorUniqueIds.at(i) = (int)hexLong;
    }
    idFile.close();

    int idx;
    std::array<int, SENSORSPERCONTROLLER> selectedSensorIds;
    for (int i = 0; i < NUMCONTROLLERS; i++) {
        idx = 0;
        for (int j = 0; j < SENSORSPERCONTROLLER; j++) {
            selectedSensorIds.at(idx) = SensorUniqueIds.at(i * SENSORSPERCONTROLLER + j);
            idx++;
        }

        // inject hardcoded unique IDs into controllers
        /*
        Storing unique id values in variables to mimic the real use case where
        there are 2 ints at 2 different addresses in the controller's memory.
        We could use a long here, since the adresses are consecutive,
        but I designed it this way to keep consistency with the datasheet.
        */
        inv->controllers.at(i)->addresses[cUNIQUE_ID1] = ControllerUniqueIds.at(i*2);
        inv->controllers.at(i)->addresses[cUNIQUE_ID2] = ControllerUniqueIds.at((i*2) + 1);

        // inject hardcoded unique IDs into sensors
        /*
        Storing unique id values in variables to mimic the real use case where
        there are 4 bytes at 4 different addresses in the sensor's memory.
        Storing the unique id values this way is the solution to the "work around":
        */
        int shift;
        for (int j = 0; j < SENSORSPERCONTROLLER; j++) {
            shift = 0;
            for (int addr : {sUNIQUE_ID1, sUNIQUE_ID2, sUNIQUE_ID3, sUNIQUE_ID4}) {
                inv->controllers.at(i)->sensors.at(j)->addresses[addr] = (std::byte)(selectedSensorIds.at(j) >> shift);
                shift += 8;
            }
        }
    }
}

/*
linear function to convert the 16-bit temperature value read from
TEMP_1/TEMP_2 which maps codes [0x0000, 0xFFFF] to Celsius [-100.0C to +100.0C].
*/
double convertTemp(uint t1, uint t2) {
    int temp = t1 + (t2 << 8);
    return (double)temp / (65535.0 / 2000.0) - 1000.0;
}

void takeInventory(Inventory *inv) {

    std::ofstream inventoryFile;
    inventoryFile.open ("inventory.txt");
    inventoryFile << "Inventory:\n\n";

    // Getting all chips
    std::vector<int> sensors = getAllSensors();
    std::vector<int>controllers = getAllControllers();
    sensorPortions portions = getPortions();

    // Loop through controllers
    int uid1;
    int uid2;
    for (int i : controllers) {
        // First for each controller we output its Unique ID, followed by Unique IDs of its sensors
        // In a real system the readController methods would be used
        uid1 = readController(std::byte(i), cUNIQUE_ID1);
        uid2 = readController(std::byte(i), cUNIQUE_ID2);
        // For the sake of the exerice, we just read our hardcoded values
        uid1 = inv->controllers.at(i)->addresses[cUNIQUE_ID1];
        uid2 = inv->controllers.at(i)->addresses[cUNIQUE_ID2];
        // output contoller Unique ID to txt file
        inventoryFile << "Controller " << std::dec << i << " Unique ID: ";
        inventoryFile << "0x" << std::hex << uid2 << uid1 << "\n";

        // Now we output sensor Unique IDs
        // This is what the "real world" implementation would look like
        int shift = 0;
        std::vector<int> sensorData(SENSORSPERCONTROLLER);
        std::vector<int> tempSensorData(SENSORSPERCONTROLLER);
        for (int addr : {sUNIQUE_ID1, sUNIQUE_ID2, sUNIQUE_ID3, sUNIQUE_ID4}) {
            tempSensorData = readSensorDataPerController((std::byte)i, sensors, portions, addr);
            for (int j : sensors) {
                sensorData.at(j) += tempSensorData.at(j) << shift;
            }
            shift += 8;
        }
        // However for the sake of the exercise
        // we will overrite sensorData with the hardcoded data from the txt file
        std::fill(sensorData.begin(), sensorData.end(), 0);
        for (int j : sensors) {
            shift = 0;
            for (int addr : {sUNIQUE_ID1, sUNIQUE_ID2, sUNIQUE_ID3, sUNIQUE_ID4}) {
                sensorData.at(j) += (int)(inv->controllers.at(i)->sensors.at(j)->addresses[addr]) << shift;
                shift += 8;
            }
        }

        // output sensor Unique ID to txt file
        for (int j : sensors) {
            inventoryFile << "    " << "Sensor " << j << " Unique ID: ";
            inventoryFile << "0x" << std::hex << sensorData.at(j) << "\n";
        }
        inventoryFile << "\n";
    }
    inventoryFile.close();
}

void readAllTemperatureSensorData() {
    
    // Getting all chips
    std::vector<int> sensors = getAllSensors();
    std::vector<int>controllers = getAllControllers();
    sensorPortions portions = getPortions();

    // We will read TEMP_1 and TEMP_2 data for each sensor
    // We can map this data to Celsius [-100.0C to +100.0C]
    std::vector<int> temp1Data(NUMCONTROLLERS * SENSORSPERCONTROLLER);
    std::vector<int> temp2Data(NUMCONTROLLERS * SENSORSPERCONTROLLER);

    // TEMP_1 data for each sensor of controller i
    temp1Data = readMultipleControllersSensorData(controllers, sensors, portions, TEMP_1);
    // TEMP_2 data for each sensor of controller i
    temp2Data = readMultipleControllersSensorData(controllers, sensors, portions, TEMP_2);

    std::ofstream tempDataFile;
    tempDataFile.open ("temperature.txt");
    tempDataFile << "Tempatures:\n\n";
    
    for (int i = 0; i < NUMCONTROLLERS * SENSORSPERCONTROLLER; i++) {
        // Map the temperature data to a Celsius value and output in the txt file
        double data = convertTemp(temp1Data.at(i), temp2Data.at(i));
        tempDataFile << (int)(data / 10) << "." << abs((int)(data) % 10) << " °C \n";
    }

    tempDataFile.close();
}