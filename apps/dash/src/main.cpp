#include <SFML/Graphics.hpp>
#include "gauge.hpp"
#include "rcan.hpp"
#include "pid.hpp"
#include "telemetry.hpp" // Our new processing function home
#include <iostream>
#include <vector>
#include "can.hpp"
int main()
{
    sf::Font font("media/Designer.otf"); 
    
    std::vector<PID*> pids;
    
    PID coolant(0, 2, 0, 215, "media/engine-coolant.png");
    pids.emplace_back(&coolant);
    
    PID map(1, 0, 0, 255, "media/engine-coolant.png");
    pids.emplace_back(&map);
    
    PID tach(0, 0, 0, 16384, "media/engine-coolant.png");
    pids.emplace_back(&tach);
    
    PID speed(1, 6, 0, 255, "media/engine-coolant.png");
    pids.emplace_back(&speed);

    rCan can("vcan0");
    
    sf::VideoMode windowSize({1024, 600});
    sf::RenderWindow window(windowSize, "Kamry Dashboard", sf::State::Windowed);
    window.setFramerateLimit(30);
    
    gauge gauge1(512, 175, 100,   0,  -90, 20, &coolant,  4, 150, 210, &font);
    gauge gauge2(  0, 600, 300,  -2,  -88, 20, &speed,   15,   0, 140, &font);
    gauge gauge3(1024, 600, 300,-178,  -92, 20, &tach,    10,   0,   9, &font);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        // 1. Read raw incoming frame states off the wire
        can.canRead();

        // 2. Clear out manual math loops! Let the processor distribute the updates.
        // Assumes can.canRead() populates the last active ID and raw buffer packet.
        TelemetryParser::processIncomingMessage(can.current_id, can.current_raw_bytes, pids);
        
        // Render Sequence
        window.clear();
        
        gauge1.update();
        gauge2.update();
        gauge3.update();

        gauge1.render(window);
        gauge2.render(window);
        gauge3.render(window);

        window.display();
    }
}