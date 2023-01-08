#include <iostream>

#include "bus.h"

// Stub out the write/read methods provided by the existing drivers

void write(std::byte busId, uint addr, uint data) {
    return;
}

uint read(std::byte busId, uint addr) {
    return 0;
}