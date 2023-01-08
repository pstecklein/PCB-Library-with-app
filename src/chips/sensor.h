#include <unordered_map>

// Sensor struct - holds address data
struct sensor {
    std::unordered_map<int, std::byte> addresses;
    int addr_sel_pins; // address-select pins to configure unique "Bus ID"
    sensor(int pins);
};