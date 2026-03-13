#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "telemetry.h"
#include "buffer.h"
#include "crc.h"
#include "events.h"
#include "logger.h" // Our new dynamic memory logger

// --- OUR DEDICATED EVENT HANDLERS ---
void Handle_OverVoltage(uint16_t meter_id, uint32_t voltage) {
    printf("\n>>> [CRITICAL ALARM] Meter %03d reported Over-Voltage: %d V\n", meter_id, voltage);
    printf(">>> ACTION: Tripping substation relay to protect grid...\n\n");
    
    // Log it to the Heap!
    Log_Add_Fault(meter_id, "OVER-VOLTAGE SPIKE");
}

void Handle_PowerOutage(uint16_t meter_id, uint32_t ignored_val) {
    (void)ignored_val; // Silence unused variable warning
    printf("\n>>> [BLACKOUT ALERT] Meter %03d lost power! Dispatching crew...\n\n", meter_id);
    
    // Log it to the Heap!
    Log_Add_Fault(meter_id, "TOTAL BLACKOUT");
}
// ------------------------------------

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
    printf("--- Smart Grid Gateway: Booting Final Architecture ---\n\n");
    srand(time(NULL));

    // 1. REGISTER OUR CALLBACKS
    Event_Register(EVENT_OVER_VOLTAGE, Handle_OverVoltage);
    Event_Register(EVENT_POWER_OUTAGE, Handle_PowerOutage);

    // 2. INITIALIZE BUFFER
    RingBuffer_t rx_buffer;
    Buffer_Init(&rx_buffer);
    uint8_t incoming_checksums[5];

    printf("[SIMULATION] Receiving high-speed network data...\n");

    // 3. THE PRODUCER (Hardware simulation)
    for (int i = 0; i < 5; i++) {
        uint16_t mock_id = 100 + rand() % 900;
        uint8_t mock_volt = 220 + (rand() % 40);
        
        // Force a blackout on packet 4
        bool blackout = (i == 3) ? true : false;
        bool over_volt = (mock_volt > 250);
        
        uint32_t data = Generate_Mock_Packet(mock_id, mock_volt, 30, over_volt, blackout);
        
        // Sender calculates checksum
        uint8_t chksum = Calculate_Checksum(data);

        // Corrupt packet 3 to test our CRC math
        if (i == 2) {
            printf("  [!] EMI SPIKE! Corrupting packet 3 in transit...\n");
            data ^= (1 << 15);
        }

        incoming_checksums[i] = chksum;
        Buffer_Write(&rx_buffer, data);
    }

    printf("\n--- Processing Buffer ---\n");
    
    // 4. THE CONSUMER (Main CPU Loop)
    uint32_t data_to_process = 0;
    int packet_index = 0;
    SmartMeterPacket_t parser;

    while (Buffer_Read(&rx_buffer, &data_to_process)) {
        
        // Validate Data Integrity First
        if (!Validate_Packet(data_to_process, incoming_checksums[packet_index])) {
            printf("[ERROR] Checksum Mismatch! Packet dropped.\n");
            packet_index++;
            continue; 
        }

        // Parse using our Union
        parser.raw_payload = data_to_process;
        printf("Parsed -> Meter ID: %03d | Volts: %d V\n", parser.fields.meter_id, parser.fields.voltage_level);

        // Trigger Event Callbacks if flags are set
        if (parser.fields.over_volt_flg) {
            Event_Trigger(EVENT_OVER_VOLTAGE, parser.fields.meter_id, parser.fields.voltage_level);
        }
        if (parser.fields.power_out_flg) {
            Event_Trigger(EVENT_POWER_OUTAGE, parser.fields.meter_id, 0);
        }

        packet_index++;
    }

    printf("\n--- System Idle. Shutting Down. ---\n");
    
    // 5. PRINT REPORT AND FREE MEMORY
    Log_Print_All();
    Log_Cleanup();

    return 0;
}
