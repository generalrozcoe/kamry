#include <SFML/Graphics.hpp>
#include "gauge.hpp"
#include "rcan.hpp"
#include <iostream>
#include <vector>
#include "can.hpp"
#include <printf.h>

int main()
{
    //media loading
    sf::Font font("media/Designer.otf");
    



    rCan can("vcan0");
    std::vector<gauge*> gauges;

    sf::VideoMode windowSize({1024, 600});
    sf::RenderWindow window(windowSize, "Kamry Dashboard", sf::State::Windowed);
    window.setFramerateLimit(30);
    //Coolant gauge
    gauge coolant(512, 175, 100, 0, -90, 20, 4, 60, 105, &font);
    gauges.push_back(&coolant);
    //bar::bar(float originX, float originY, float width, int vFrom, int vTo, int from, int to, sf::Font *font)

    bar coolantB(20, 175, 50, 0, 0,60,105,200, &font,"media/coolant.png" );

    //Speedometer gauge
    gauge speed(0, 600, 300, -2, -88, 20, 15, 0, 140, &font);
    gauges.push_back(&speed); 
    gauge tach(1024, 600, 300, -178, -92, 20, 10, 0, 9000, &font);
    gauges.push_back(&tach);
    std::cout << "init fine" << std::endl;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // 1. Read raw incoming frame states off the wire
        // --- SAFE CAN PROCESSING BLOCK ---
        uint32_t incoming_id = 0;
        uint8_t scratch_pad[8] = {0};
        KamryCAN::ECU::Primary1Frame primary1frame = {}; // Zero initialize

        if (can.canRead(incoming_id, scratch_pad))
        {

            // 1. PROCESS PRIMARY 1 COOLDOWN/RPM VARIABLES
            if (incoming_id == KamryCAN::ECU::PRIMARY_1) //
            {
                //std::cout << "msg rcvdin loop" << std::endl;
                // This copies exactly 8 bytes safely without risking array index boundary leaks.
                std::memcpy(primary1frame.bytes, scratch_pad, 8);
                /*
                std::cout << "Received Frame 0x" << std::hex << incoming_id << ": ";
                for (int i = 0; i < 8; ++i)
                {
                    std::cout << "[" << std::dec << i << "]:0x" << std::hex << (int)primary1frame.bytes[i] << " ";
                }
                std::cout << std::dec << std::endl;
                */
                // Safely translate the variables out of the packed struct
                coolant.setValue( ( static_cast<float>(primary1frame.data.coolant_temp)));
                coolantB.setValue( (static_cast<float>(primary1frame.data.coolant_temp))); //
                tach.setValue(  static_cast<float>(primary1frame.data.rpm));            //

            }

            // 2. PROCESS OTHER ENGINE VARIABLES
            else if (can.current_id == KamryCAN::ECU::ENGINE_VARS) //
            {
                KamryCAN::ECU::EngineVarsFrame frame = {}; //

                // 🛠️ THE FIX: Safe memory replication
                std::memcpy(frame.bytes, can.current_raw_bytes, 8);

                // Example: throttle = static_cast<float>(frame.data.throttle_pos) / 100.0f;
            }
        }

        // Render Sequence
        window.clear(sf::Color::Black);
        //std::cout<<"render"<<std::endl;
        for (auto &g : gauges)
        {
            g->update();
        }
        coolantB.update();
        for (auto &g : gauges)
        {
            g->render(window);
        };
        coolantB.render(window);
        window.display();
    }
}