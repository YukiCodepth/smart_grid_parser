#include "events.h"
#include <stddef.h> 

// Dispatch Table
static FaultCallback_t event_registry[EVENT_MAX] = {NULL};

void Event_Register(EventType_t event, FaultCallback_t callback) {
    if (event < EVENT_MAX) {
        event_registry[event] = callback; 
    }
}

void Event_Trigger(EventType_t event, uint16_t meter_id, uint32_t fault_value) {
    if (event < EVENT_MAX && event_registry[event] != NULL) {
        event_registry[event](meter_id, fault_value);
    }
}
