#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

// A node in our Linked List
typedef struct FaultNode {
    uint16_t meter_id;
    const char* fault_type;
    struct FaultNode* next; // Pointer to the next fault in the chain
} FaultNode_t;

// API Prototypes
void Log_Add_Fault(uint16_t meter_id, const char* fault_type);
void Log_Print_All(void);
void Log_Cleanup(void);

#endif // LOGGER_H
