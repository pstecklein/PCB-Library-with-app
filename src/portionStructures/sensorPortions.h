// sensor portions to be used for readSensor method

// Relevant portions for sensorBus operations
struct sensorPortions {
    int busId;
    int addr;
    int data;
    int op;
    int status;
    void modifyPortions(int busId, int addr, int data, int op, int status);
};