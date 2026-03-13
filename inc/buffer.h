#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 16 // Holds 16 packets max

// The Circular Buffer Structure
typedef struct {
    uint32_t data[BUFFER_SIZE]; // Array to hold the raw hex payloads
    int head;                   // Pointer for where to WRITE new data
    int tail;                   // Pointer for where to READ old data
    int count;                  // How many items are currently in the buffer
} RingBuffer_t;

// Function Prototypes (The API)
void Buffer_Init(RingBuffer_t *buf);
bool Buffer_Write(RingBuffer_t *buf, uint32_t payload);
bool Buffer_Read(RingBuffer_t *buf, uint32_t *payload_out);

#endif // BUFFER_H
