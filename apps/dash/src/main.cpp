#include <SFML/Graphics.hpp>
#include "gauge.hpp"
#include "can.hpp"
#include <iostream>
#include "pid.hpp"
#include <map>
#include <vector>
#include <algorithm>
int main()
{
	sf::Font font("media/Designer.otf"); 
	//Making array of pids
	std::vector <PID*> pids;
	//Coolant pid
	PID coolant(0,2, 0,215, (std::string)"media/engine-coolant.png");
	pids.emplace_back(&coolant);
	//Manifold Absolute Pressure pid
	PID map(1,0, 0,255, (std::string)"media/engine-coolant.png");
	pids.emplace_back(&map);
	//Tachometer pid
	PID tach(0,0, 0,16384, (std::string)"media/engine-coolant.png");
	pids.emplace_back(&tach);
	//Speedometer pid
	PID speed(1,6, 0,255, (std::string)"media/engine-coolant.png");
	pids.emplace_back(&speed);
	rCan can("vcan0");
	
	
//    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
//    sf::RenderWindow window(desktopMode, "SFML Fullscreen", sf::State::Fullscreen);
	
	//Create Window
	sf::VideoMode windowSize({1024, 600});
    sf::RenderWindow window(windowSize, "Small Window", sf::State::Windowed);
    window.setFramerateLimit(30);
	
	
	
	gauge gauge1(512,175, 100,0, -90,20, &coolant, 4, 150,210, &font);
	gauge gauge2(0,600, 300,-2, -88,20, &speed, 15, 0,140, &font);
	gauge gauge3(1024,600, 300,-178, -92,20, &tach, 10, 0,9, &font);

//	gauge gauge3(512,300, 100,45,-45,20);
//	gauge gauge4(512,300, 100,225,315,20);
	
	
    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
        }
		
		
		
		// 1. Read the bus and fill the 2D array
can.canRead();

// 2. Process every PID based on its array coordinates
// 2. Process every PID based on its array coordinates
for (PID* p : pids) {
    int msgIdx = p->ID; // Your current code uses ID as index (0, 1, 2...)
    int byteIdx = p->offset;

    // Safety check for the [10][8] data array
    if (msgIdx < 0 || msgIdx >= 10) continue;

    // Reconstruct the 16-bit value (Big Endian)
    uint16_t raw = (can.data[msgIdx][byteIdx] << 8) | can.data[msgIdx][byteIdx + 1];
    // Math Switch for rusEFI Sensors
    if (msgIdx == 0) { // Message ID 512 (0x200)
        switch (byteIdx) {
            case 0: // RPM
                p->data = raw/1000.0; 
                break;
            case 2: // Coolant (CLT)
                p->data = (int16_t)raw / 10; // 0.1C scale
                break;
            case 4: // TPS
                p->data = raw / 100; // 0.01% scale
                break;
            case 6: // Intake Air (IAT)
                p->data = (int16_t)raw / 10; // 0.1C scale
                break;
        }
    } 
    else if (msgIdx == 1) { // Message ID 513 (0x201)
        switch (byteIdx) {
            case 0: // MAP
                p->data = raw / 10; // 0.1 kPa scale
                break;
            case 2: // Battery Voltage
                p->data = raw / 1000.0f; // mV to Volts
                break;
            case 4: // Barometer
                p->data = raw / 10; // 0.1 kPa scale
                break;
            case 6: // Speed (VSS)
                // Convert kph to mph if needed: (raw / 100.0f) * 0.621371f
                p->data = raw / 100.0f; 
                break;
        }
    }
    else if (msgIdx == 2) { // Message ID 514 (0x202)
        switch (byteIdx) {
            case 0: // Fuel Pressure
                p->data = raw / 10; // 0.1 kPa scale
                break;
            case 2: // Oil Pressure
                p->data = raw / 10; // 0.1 kPa scale
                break;
            case 4: // Fuel Consumption
                p->data = raw / 10; // L/100km
                break;
        }
    }
}
		
        window.clear();
		
		gauge1.update();
		gauge2.update();
		gauge3.update();

		gauge1.render(window);
		
		gauge2.render(window);
		gauge3.render(window);
//		gauge4.render(window);
		

        window.display();
    }
}
