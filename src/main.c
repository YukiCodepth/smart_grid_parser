#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "telemetry.h"
#include "buffer.h"
#include "crc.h" // Include our new validation module

uint32_t Generate_Mock_Packet(uint16_t id, uint8_t volt, uint8_t current, bool over_volt, bool pwr_out) {
    uint32_t raw = 0;
    raw |= (id & 0x0FFF);
    raw |= ((uint32_t)volt << 12);
    raw |= ((uint32_t)current << 20);
    raw |= ((uint32_t)over_volt << 28);
    raw |= ((uint32_t)pwr_out << 29);
    return raw;
}

int main() {
    printf("--- Smart Grid Gateway: Booting with Checksum Validation ---\n\n");
    srand(time(NULL));

    RingBuffer_t rx_buffer;
    Buffer_Init(&rx_buffer);

    printf("[SIMULATION] Receiving network data...\n");

    // We will use parallel arrays to simulate receiving the payload AND its checksum
    uint8_t incoming_checksums[5];

    // THE PRODUCER
    for (int i = 0; i < 5; i++) {
        uint16_t mock_id = 100 + rand() % 900;
        uint8_t mock_volt = 220 + (rand() % 40);
        uint32_t data = Generate_Mock_Packet(mock_id, mock_volt, 30, false, false);
        
        // Sender calculates the correct checksum before transmitting
        uint8_t chksum = Calculate_Checksum(data);

        // SIMULATE INTERFERENCE: Force corrupt the 3rd packet (index 2)
        if (i == 2) {
            printf("  [!] EMI SPIKE! Corrupting packet 3 in transit...\n");
            data ^= (1 << 15); // Flip bit 15 (corrupts the voltage data)
        }

        incoming_checksums[i] = chksum;
        
        // Push raw data to buffer (in a real system, you'd buffer a struct holding both)
        Buffer_Write(&rx_buffer, data);
    }

    printf("\n--- Processing Buffer ---\n");
    
    uint32_t data_to_process = 0;
    int packet_index = 0;
    SmartMeterPacket_t parser;

    // THE CONSUMER
    while (Buffer_Read(&rx_buffer, &data_to_process)) {
        
        // 1. VALIDATE DATA FIRST
        uint8_t received_chksum = incoming_checksums[packet_index];
        
        if (!Validate_Packet(data_to_process, received_chksum)) {
            printf("[ERROR] Checksum Mismatch! Packet dropped to prevent grid failure.\n");
            packet_index++;
            continue; // Skip the rest of the loop and grab the next packet!
        }

        // 2. ONLY PARSE IF VALID
        parser.raw_payload = data_to_process;
        printf("Meter ID: %03d | Volts: %d V | Data Intact and Verified.\n", 
            parser.fields.meter_id, 
            parser.fields.voltage_level
        );
        packet_index++;
    }

    printf("\n--- System Idle. ---\n");
    return 0;
}
