#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h> // We must include this to use fixed-width integers

/* * 1. The Bit-Field Struct
 * We define exactly how the 32 bits (4 bytes) of our payload are divided.
 */
typedef struct {
    uint32_t meter_id      : 12; // Bits 0-11: ID from 0 to 4095
    uint32_t voltage_level : 8;  // Bits 12-19: Voltage (scaled, 0-255)
    uint32_t current_load  : 8;  // Bits 20-27: Current load (0-255)
    uint32_t over_volt_flg : 1;  // Bit 28: 1 if Over-Voltage detected
    uint32_t power_out_flg : 1;  // Bit 29: 1 if Power Outage detected
    uint32_t reserved      : 2;  // Bits 30-31: Reserved for future use
} MeterData_t;

/* * 2. The Union
 * This overlays the raw 32-bit integer exactly on top of our bit-field struct.
 */
typedef union {
    uint32_t raw_payload; // We write the incoming network data here
    MeterData_t fields;   // We read the human-usable data from here
} SmartMeterPacket_t;

#endif // TELEMETRY_H
