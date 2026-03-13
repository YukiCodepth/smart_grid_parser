#include <stdio.h>
#include <stdint.h>
#include "telemetry.h" // Include the header we just made!

int main() {
    printf("--- Smart Grid Telemetry Parser Booting ---\n\n");

    // 1. Create an instance of our union
    SmartMeterPacket_t packet;

    // 2. Simulate receiving a 32-bit raw payload from the network.
    // Let's pretend a meter just transmitted the hex value: 0x11A78123
    uint32_t simulated_rx_data = 0x11A78123; 
    
    // 3. Dump the raw data straight into the union's integer side
    packet.raw_payload = simulated_rx_data;

    // 4. Read the data out using the struct side! 
    // Notice how we don't have to do any bit-shifting or masking manually.
    printf("Raw Hex Received: 0x%08X\n", packet.raw_payload);
    printf("-------------------------------------------\n");
    printf("Meter ID        : %d\n", packet.fields.meter_id);
    printf("Voltage Level   : %d V\n", packet.fields.voltage_level);
    printf("Current Load    : %d A\n", packet.fields.current_load);
    
    // Print the fault flags. We use a ternary operator (condition ? true : false) 
    // to print "YES" or "NO" instead of just 1 or 0.
    printf("Over-Voltage    : %s\n", packet.fields.over_volt_flg ? "YES ALARM!" : "Normal");
    printf("Power Outage    : %s\n", packet.fields.power_out_flg ? "YES ALARM!" : "Normal");
    printf("-------------------------------------------\n\n");

    return 0;
}
