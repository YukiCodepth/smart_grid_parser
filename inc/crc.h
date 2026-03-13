#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stdbool.h>

// Function Prototypes
uint8_t Calculate_Checksum(uint32_t payload);
bool Validate_Packet(uint32_t payload, uint8_t received_checksum);

#endif // CRC_H
