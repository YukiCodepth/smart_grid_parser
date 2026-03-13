#include "buffer.h"
#include <stdio.h>

// Initialize the buffer by setting all pointers to 0
void Buffer_Init(RingBuffer_t *buf) {
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
}

// Write data INTO the buffer (like a UART interrupt receiving data)
bool Buffer_Write(RingBuffer_t *buf, uint32_t payload) {
    if (buf->count >= BUFFER_SIZE) {
        printf("WARNING: Buffer Full! Packet 0x%08X dropped.\n", payload);
        return false; // Buffer is full!
    }

    buf->data[buf->head] = payload;          // Store the data
    buf->head = (buf->head + 1) % BUFFER_SIZE; // Move head forward, wrap around if needed
    buf->count++;                            // Increase item count
    return true;
}

// Read data OUT OF the buffer (using a pointer to pass the data back)
bool Buffer_Read(RingBuffer_t *buf, uint32_t *payload_out) {
    if (buf->count == 0) {
        return false; // Buffer is empty!
    }

    *payload_out = buf->data[buf->tail];     // Pull the data out using the pointer
    buf->tail = (buf->tail + 1) % BUFFER_SIZE; // Move tail forward, wrap around if needed
    buf->count--;                            // Decrease item count
    return true;
}
