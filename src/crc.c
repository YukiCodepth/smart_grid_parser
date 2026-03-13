#include "crc.h"

/* * Breaks a 32-bit payload into four 8-bit chunks and XORs them.
 * This is a standard Longitudinal Redundancy Check (LRC).
 */
uint8_t Calculate_Checksum(uint32_t payload) {
    uint8_t b0 = (payload & 0xFF);           // Extract bits 0-7
    uint8_t b1 = ((payload >> 8) & 0xFF);    // Shift and extract bits 8-15
    uint8_t b2 = ((payload >> 16) & 0xFF);   // Shift and extract bits 16-23
    uint8_t b3 = ((payload >> 24) & 0xFF);   // Shift and extract bits 24-31
    
    return (b0 ^ b1 ^ b2 ^ b3);              // Bitwise XOR creates the fingerprint
}

/*
 * Compares our calculated checksum against the one received over the network.
 */
bool Validate_Packet(uint32_t payload, uint8_t received_checksum) {
    uint8_t expected = Calculate_Checksum(payload);
    return (expected == received_checksum);
}
