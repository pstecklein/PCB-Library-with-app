// Controller - relevant portions from datasheet
const int cUNIQUE_ID1 = 0x10235020;
const int cUNIQUE_ID2 = 0x10235024;
const int SENSORBUS_BUSID = 0x21112000;
const int SENSORBUS_ADDR = 0x21112001;
const int SENSORBUS_DATA = 0x21112002;
const int SENSORBUS_OP = 0x21112003;
const int SENSORBUS_STATUS = 0x21112004;

// Sensor - relevant portions from datasheet
const int sUNIQUE_ID1 = 0x20;
const int sUNIQUE_ID2 = 0x21;
const int sUNIQUE_ID3 = 0x22;
const int sUNIQUE_ID4 = 0x24;
const int TEMP_1 = 0x45;
const int TEMP_2 = 0x46;

// PCB info
const int NUMCONTROLLERS = 8;
const int SENSORSPERCONTROLLER = 4;