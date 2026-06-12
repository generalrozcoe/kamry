#include "pico/stdlib.h"
#include "hardware/gpio.h"
extern "C" {
  #include "can2040.h"
}
#include "can.hpp"         // Your unified Kamry network layout

// 🛠️ Array structures to make initializations clean
const uint INPUT_PINS[] = {10, 11, 12, 13, 14, 15};
const uint OUTPUT_PINS[] = {0, 1, 2, 3, 4, 5, 6, 7};


// Simple example of irq safe queue (this is not multi-core safe)
#define QUEUE_SIZE 128 // Must be power of 2
static struct {
    uint32_t pull_pos;
    volatile uint32_t push_pos;
    struct can2040_msg queue[QUEUE_SIZE];
} MessageQueue;

// Internal storage for can2040 module
static struct can2040 cbus;




static void
can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    if (notify == CAN2040_NOTIFY_RX) {
        // Example message filter
        uint32_t id = msg->id;
        if (id < 0x101 || id > 0x201)
            return;

        // Add to queue
        uint32_t push_pos = MessageQueue.push_pos;
        uint32_t pull_pos = MessageQueue.pull_pos;
        if (push_pos + 1 == pull_pos)
            // No space in queue
            return;
        MessageQueue.queue[push_pos % QUEUE_SIZE] = *msg;
        MessageQueue.push_pos = push_pos + 1;
    }
}
static void
PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}
void
canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = SYS_CLK_HZ, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}
void setup_hardware() {
    // Initialize standard runtime operations
    stdio_init_all();
        canbus_setup();

    // 1. Initialize and configure Input Stalk Switches
    for (uint pin : INPUT_PINS) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin); // Keeps inputs from floating when switches are open
    }

    // 2. Initialize and configure MOSFET Gate Drivers
    for (uint pin : OUTPUT_PINS) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, false); // Explicitly pull outputs LOW on startup
    }

    // NOTE: Put your can2040/SPI hardware setup functions here later!
}

// Main processing loop running on your Front Lighting Pico Node
void loop() {
    KamryCAN::BCM::LightsFrame lightingNode;

    // 1. Read Physical Stalk Switches (Inputs via gpio_get)
    lightingNode.data.low_beams  = gpio_get(10); // gp10
    lightingNode.data.high_beams = gpio_get(11); // gp11
    lightingNode.data.left_turn  = gpio_get(12); // gp12
    lightingNode.data.right_turn = gpio_get(13); // gp13
    lightingNode.data.hazards    = gpio_get(14); // gp14
    lightingNode.data.brakes     = gpio_get(15); // gp15

    // 2. Write Directly to the MOSFET Gate Transistors (Outputs via gpio_put)

    gpio_put(2, lightingNode.data.low_beams);   // gp2  (Fixed typo from low_beam)
    gpio_put(3, lightingNode.data.high_beams);  // gp3  (Fixed typo from high_beam)
    gpio_put(4, lightingNode.data.low_beams);   // gp4
    gpio_put(5, lightingNode.data.high_beams);  // gp5
bool flash_now = ((to_ms_since_boot(get_absolute_time()) / 500) % 2 == 0);

// 2. Process the lighting priority logic tree
if (lightingNode.data.hazards == true && flash_now) {
    // Hazards are pressed and the clock is even -> Flash both sides!
    gpio_put(0, 1);   // gp0 - Left Turn
    gpio_put(1, 1);   // gp1 - Right Turn
} 
else if (lightingNode.data.left_turn == true && flash_now) {
    // Only Left turn is active and clock is even
    gpio_put(0, 1);   // gp0 ON
    gpio_put(1, 0);   // gp1 OFF
} 
else if (lightingNode.data.right_turn == true && flash_now) {
    // Only Right turn is active and clock is even
    gpio_put(0, 0);   // gp0 OFF
    gpio_put(1, 1);   // gp1 ON
} 
else {
    // Default safety fallback: if it's an "odd" tick, or nothing is pressed, turn them off!
    gpio_put(0, 0);   // gp0 OFF
    gpio_put(1, 0);   // gp1 OFF
}
    // 4. Send this telemetry package over the CAN Bus to your Dashboard!
    // Assumes your CanBus global instance layout exposes a raw packet interface
        struct can2040_msg tmsg;
            tmsg.id = KamryCAN::BCM::BCM_Lights;
            tmsg.dlc = 1;
            for(int i = 0; i <8; i++){
            tmsg.data[i] = lightingNode.bytes[i];
            can2040_transmit(&cbus, &tmsg);
            }
}

// 🛠️ THE RUNNER: Standard C runtime entry point
int main() {
    setup_hardware();

    while (true) {
        loop();
        sleep_ms(10); // Throttle loop to ~100Hz so you don't slam the CAN bus
    }

    return 0;
}