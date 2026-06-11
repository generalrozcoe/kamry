#ifndef TELEMETRY_PARSER_HPP
#define TELEMETRY_PARSER_HPP

#include "can.hpp"
#include "pid.hpp"
#include <vector>

class TelemetryParser {
public:
    // Decodes raw CAN data and updates the PID objects directly
    static void processIncomingMessage(uint32_t id, const uint8_t* rawBytes, std::vector<PID*>& pids) {
        
        // 1. PROCESS rusEFI ENGINE DATA
        if (id == KamryCAN::ECU::PRIMARY_1) {
            KamryCAN::ECU::Primary1Frame frame;
            for (int i = 0; i < 8; ++i) frame.bytes[i] = rawBytes[i];

            updatePidValue(pids, 0, 0, static_cast<float>(frame.data.rpm) / 1000.0f);
            updatePidValue(pids, 0, 2, static_cast<float>(frame.data.coolant_temp) / 10.0f);
            // IAT or MAP values can be updated here as well...
        }
        
        // 2. PROCESS OTHER ECU ENGINE VARIABLES
        else if (id == KamryCAN::ECU::ENGINE_VARS) {
            KamryCAN::ECU::EngineVarsFrame frame;
            for (int i = 0; i < 8; ++i) frame.bytes[i] = rawBytes[i];

            updatePidValue(pids, 1, 0, static_cast<float>(frame.data.throttle_pos) / 100.0f);
            updatePidValue(pids, 1, 2, static_cast<float>(frame.data.battery_voltage) / 1000.0f);
            // Speed (VSS) or others...
        }

        // 3. PROCESS CHASSIS BCM DATA (1-Byte Messages)
        else if (id == KamryCAN::BCM::BCM_Lights) {
            KamryCAN::BCM::LightsFrame frame;
            frame.bytes[0] = rawBytes[0];

            // If you map BCM states to a PID or a UI boolean flag:
            // e.g., highBeamActive = frame.data.high_beams;
        }
    }

private:
    // Helper function to find the corresponding PID in your vector and update it
    static void updatePidValue(std::vector<PID*>& pids, int msgId, int offset, float newValue) {
        for (PID* p : pids) {
            if (p->ID == msgId && p->offset == offset) {
                p->data = newValue;
                return;
            }
        }
    }
};

#endif