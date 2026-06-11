#ifndef KAMRY_CAN_ECU_H
#define KAMRY_CAN_ECU_H

#include <cstdint>

namespace KamryCAN::ECU {

    enum ID : uint32_t {
        PRIMARY_1   = 0x1F0A010, 
        ENGINE_VARS = 0x200    
    };

    #pragma pack(push, 1)
    struct Primary1Data {
        uint16_t rpm;             
        uint16_t manifold_press;  
        int16_t  coolant_temp;    
        int16_t  intake_air_temp; 
    };

    struct EngineVarsData {
        uint16_t throttle_pos;    
        uint16_t battery_voltage; 
        int16_t  ignition_advance;
        uint16_t injection_ms;    
    };
    #pragma pack(pop)

    union Primary1Frame   { Primary1Data data;   uint8_t bytes[8]; };
    union EngineVarsFrame { EngineVarsData data; uint8_t bytes[8]; };
}

#endif