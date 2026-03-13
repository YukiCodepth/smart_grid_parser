#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

// Function Pointer Definition
typedef void (*FaultCallback_t)(uint16_t meter_id, uint32_t fault_value);

// Event Types
typedef enum {
    EVENT_OVER_VOLTAGE,
    EVENT_POWER_OUTAGE,
    EVENT_MAX 
} EventType_t;

// API Prototypes
void Event_Register(EventType_t event, FaultCallback_t callback);
void Event_Trigger(EventType_t event, uint16_t meter_id, uint32_t fault_value);

#endif // EVENTS_H
