#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "telemetry.h"
#include "buffer.h"

// --- Helper Function: Bit Manipulation Mastery ---
// This simulates the network hardware packing data into a 32-bit stream
uint32_t Generate_Mock_Packet(uint16_t id, uint8_t volt, uint8_t current, bool over_volt, bool pwr_out) {
    uint32_t raw = 0;
    
    // We use bitwise OR (|) and bitwise Left Shift (<<) to pack the data 
    // exactly how our Union expects it!
    raw |= (id & 0x0FFF);                   // Bits 0-11
    raw |= ((uint32_t)volt << 12);          // Bits 12-19
    raw |= ((uint32_t)current << 20);       // Bits 20-27
    raw |= ((uint32_t)over_volt << 28);     // Bit 28
    raw |= ((uint32_t)pwr_out << 29);       // Bit 29
    
    return raw;
}

int main() {
    printf("--- Smart Grid Gateway: Booting with Circular Buffer ---\n\n");

    // Seed the random number generator
    srand(time(NULL));

    // 1. Initialize our Circular Buffer
    RingBuffer_t rx_buffer;
    Buffer_Init(&rx_buffer);

    printf("[SIMULATION] Receiving high-speed burst of meter data...\n");

    // 2. THE PRODUCER: Simulate an interrupt receiving 5 packets rapidly
    for (int i = 0; i < 5; i++) {
        // Generate some fake but realistic data
        uint16_t mock_id = 100 + rand() % 900;      // ID between 100-999
        uint8_t mock_volt = 220 + (rand() % 40);    // Voltage between 220-259V
        uint8_t mock_current = 10 + (rand() % 50);  // Current between 10-59A
        bool mock_fault = (mock_volt > 250);        // Fault if voltage > 250V
        
        uint32_t incoming_data = Generate_Mock_Packet(mock_id, mock_volt, mock_current, mock_fault, false);
        
        // Push to buffer!
        Buffer_Write(&rx_buffer, incoming_data);
        printf("  -> Hardware pushed raw hex: 0x%08X to buffer.\n", incoming_data);
    }
    printf("[SIMULATION] Burst complete. Buffer contains %d items.\n\n", rx_buffer.count);


    // 3. THE CONSUMER: The Main CPU Loop
    printf("--- Processing Buffer ---\n");
    
    uint32_t data_to_process = 0;
    SmartMeterPacket_t parser;

    // Loop continuously as long as Buffer_Read returns true (meaning it found data)
    // Notice we pass the POINTER (&data_to_process) so the function can modify our variable!
    while (Buffer_Read(&rx_buffer, &data_to_process)) {
        
        // Dump the raw hex into our union
        parser.raw_payload = data_to_process;

        // Magically read out the structured data
        printf("Meter ID: %03d | Volts: %d V | Amps: %d A | Status: %s\n", 
            parser.fields.meter_id, 
            parser.fields.voltage_level, 
            parser.fields.current_load,
            parser.fields.over_volt_flg ? "FAULT: OVER-VOLTAGE!" : "OK"
        );
    }

    printf("\n--- Buffer Empty. System Idle. ---\n");
    return 0;
}
