#include "rcan.hpp"
#include <iostream>
#include <vector>
#include "can.hpp"
#include <arpa/inet.h>
#include <chrono>
#include <thread>

int main()
{
    rCan can("vcan0");
    KamryCAN::ECU::Primary1Frame simFrame;
    static uint16_t simulated_rpm = 1000;
        simFrame.data.coolant_temp = 90; // Steady 90°C


while(1){
// 1. Instantiate the frame

// 2. Simulate engine data modifications via the .data struct fields
// Let's increment RPM cleanly on every tick
simulated_rpm += 50; 
if (simulated_rpm > 7000) simulated_rpm = 1000; // Reset at redline

simFrame.data.rpm = simulated_rpm;
simFrame.data.coolant_temp +=1;
if(simFrame.data.coolant_temp >100){simFrame.data.coolant_temp = 60;};


simFrame.data.manifold_press = 1013; // Atmospheric baseline

// 3. Hand the completely packed bytes array off to your writer function
// Assuming your rCan write takes an ID and a data pointer:
can.canWrite(KamryCAN::ECU::PRIMARY_1, simFrame.bytes);
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Sleep for 500 milliseconds
        }
}