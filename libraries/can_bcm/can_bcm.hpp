#ifndef KAMRY_CAN_BCM_H
#define KAMRY_CAN_BCM_H

#include <cstdint>

namespace KamryCAN::BCM {

    // ==========================================
    // 1. BCM CAN MESSAGE IDs
    // ==========================================
    enum BcmID : uint32_t {
        BCM_Lights      = 0x310, // Headlights, indicators, brakes
        BCM_RearDoor    = 0x312, // Doors, windows, seatbelts
        BCM_FrontDoor   = 0x315  // Requests sent to the BCM (e.g., lock doors)
    };

    // ==========================================
    // 2. DATA STRUCTURE DEFINITIONS (Bit-Packed)
    // ==========================================
    
    #pragma pack(push, 1) // Force tight 1-byte data alignment

    // Message 1: Lighting States (Packed into Byte 0 and Byte 1)
    struct Lights {
        // --- BYTE 0 ---
        uint8_t low_beams      : 1; // 1 bit (0 = off, 1 = on)
        uint8_t high_beams     : 1; // 1 bit
        uint8_t left_turn      : 1; // 1 bit
        uint8_t right_turn     : 1; // 1 bit
        uint8_t brake_lights   : 1; // 1 bit
        uint8_t reverse_lights : 1; // 1 bit
        uint8_t fog_lights     : 1; // 1 bit
        uint8_t hazard_active  : 1; // 1 bit -> Total 8 bits (1 Byte)
    };

    struct RearDoor {
        // --- BYTE 0: Door Switches ---
        uint8_t window_up    : 1; // 1 = Open, 0 = Closed
        uint8_t window_down    : 1; // 1 = Open, 0 = Closed
        uint8_t door_lock    : 1; // 1 = Open, 0 = Closed
        uint8_t door_unlock    : 1; // 1 = Open, 0 = Closed
        uint8_t padding : 4;
    };
    struct FrontDoor {
        // --- BYTE 0: Door Switches ---
        uint8_t window_up    : 1; // 1 = Open, 0 = Closed
        uint8_t window_down    : 1; // 1 = Open, 0 = Closed
        uint8_t door_lock    : 1; // 1 = Open, 0 = Closed
        uint8_t door_unlock    : 1; // 1 = Open, 0 = Closed
        uint8_t mirror_up    : 1; // 1 = Open, 0 = Closed
        uint8_t mirror_down    : 1; // 1 = Open, 0 = Closed
        uint8_t mirror_left    : 1; // 1 = Open, 0 = Closed
        uint8_t mirror_right    : 1; // 1 = Open, 0 = Closed

    };
    #pragma pack(pop)

    // ==========================================
    // 3. UNIONS FOR HARDWARE TRANSLATION
    // ==========================================
    union LightsFrame {
        Lights data;
        uint8_t     bytes[8];
    };

    union RearFrame {
        RearDoor data;
        uint8_t   bytes[8];
    };
    union FrontFrame {
        FrontDoor data;
        uint8_t   bytes[8];
    };

} // namespace KamryCAN

#endif // KAMRY_CAN_H