#include <stdio.h>        //
#include "pico/stdlib.h"  //
#include "hardware/gpio.h"

extern "C" {
  #include "can2040.h"
}
#include "can.hpp"

static struct can2040 cbus;
enum pinID : int
{
    can_rx = 4,
    can_tx = 5,
    left_turn = 0,
    right_turn = 1,
    left_brake_low = 2,
    right_brake_low = 3,
    left_brake_high = 8,
    right_brake_high = 9,

    fuel_pump = 6,
    fuel_sender = 7

};
const uint INPUT_PINS[] = {fuel_sender};
const uint OUTPUT_PINS[] = {left_turn, right_turn, left_brake_low, right_brake_low, left_brake_high, right_brake_high, fuel_pump};

// --- Function Prototypes ---
static void can2040_rx_callback(struct can2040 *cd, uint32_t cb_notify, struct can2040_msg *msg);
static void pio_irq_handler(void);
    KamryCAN::BCM::LightsFrame lightingNode;

int main()
{
    stdio_init_all(); //

    // Setup can2040 configuration and attach our rx callback mechanism
    uint32_t bitrate = 500000;    // 500kbps CAN standard layout speed
    uint32_t sys_clk = 125000000; // Native clock speed of the platform
    uint gpio_rx = 4;             // Your node's physical CAN RX pin layout
    uint gpio_tx = 5;             // Your node's physical CAN TX pin layout

    can2040_setup(&cbus, 0); // Initialize core engine using PIO block 0
    can2040_callback_config(&cbus, can2040_rx_callback);

    // 3. Configure and bind the hardware Interrupt Request Handler (IRQ)
    irq_set_exclusive_handler(PIO0_IRQ_0, pio_irq_handler);
    irq_set_enabled(PIO0_IRQ_0, true);

    // 4. Launch the state machine execution loop
    can2040_start(&cbus, sys_clk, bitrate, gpio_rx, gpio_tx);
    for (uint pin : INPUT_PINS)
    {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin); // Keeps inputs from floating when switches are open
    }

    // 2. Initialize and configure MOSFET Gate Drivers
    for (uint pin : OUTPUT_PINS)
    {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, false); // Explicitly pull outputs LOW on startup
    }

    while (1)
    { //

        if (lightingNode.data.low_beams || lightingNode.data.high_beams){
            gpio_put(left_brake_low, 1);
            gpio_put(right_brake_low, 1);
        }

        gpio_put(left_brake_high, lightingNode.data.brakes);
        gpio_put(right_brake_high, lightingNode.data.brakes);

        bool flash_now = ((to_ms_since_boot(get_absolute_time()) / 500) % 2 == 0);

        // 2. Process the lighting priority logic tree
        if (lightingNode.data.hazards == true && flash_now)
        {
            // Hazards are pressed and the clock is even -> Flash both sides!
            gpio_put(left_turn, 1);  // gp0 - Left Turn
            gpio_put(right_turn, 1); // gp1 - Right Turn
        }
        else if (lightingNode.data.left_turn == true && flash_now)
        {
            // Only Left turn is active and clock is even
            gpio_put(left_turn, 1);  // gp0 ON
            gpio_put(right_turn, 0); // gp1 OFF
        }
        else if (lightingNode.data.right_turn == true && flash_now)
        {
            // Only Right turn is active and clock is even
            gpio_put(left_turn, 0);  // gp0 OFF
            gpio_put(right_turn, 1); // gp1 ON
        }
        else
        {
            // Default safety fallback: if it's an "odd" tick, or nothing is pressed, turn them off!
            gpio_put(left_turn, 0);  // gp0 OFF
            gpio_put(right_turn, 0); // gp1 OFF
        }

    } //
}

/**
 * can2040 Hardware Interrupt Hook
 */
static void pio_irq_handler(void)
{
    can2040_pio_irq_handler(&cbus);
}

/**
 * Background Interrupt Callback triggered automatically whenever a new CAN packet arrives
 */
static void can2040_rx_callback(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    if (notify & CAN2040_NOTIFY_RX)
    {
        // Map the frame payload if the arriving ID matches the compiled enum from can.hpp
        if (msg->id == KamryCAN::BCM::BCM_Lights)
        {
            // Overlay incoming data directly into our global structure bitfields
            for (int i = 0; i < 8; i++)
            {
                lightingNode.bytes[i] = msg->data[i];
            }
        }
    }
}
