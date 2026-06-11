#ifndef KAMRY_CAN_H
#define KAMRY_CAN_H

// 1. Declare the master namespace umbrella
namespace KamryCAN {
    // We leave this empty here; it just establishes the root name for the compiler
}

// 2. Pull in your split modular files
#include "can_bcm.hpp"
#include "can_ecu.hpp"

#endif // KAMRY_CAN_H