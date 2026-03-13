#include "logger.h"
#include <stdio.h>
#include <stdlib.h> // Required for malloc and free

// The "head" is the start of our linked list. It starts completely empty (NULL).
static FaultNode_t* head = NULL;

void Log_Add_Fault(uint16_t meter_id, const char* fault_type) {
    // 1. Ask the OS for exactly enough Heap memory to hold one FaultNode
    FaultNode_t* new_node = (FaultNode_t*)malloc(sizeof(FaultNode_t));
    
    // 2. ALWAYS check if malloc failed (Out of Memory)
    if (new_node == NULL) {
        printf("[CRITICAL] Out of Heap Memory! Cannot log fault.\n");
        return;
    }
    
    // 3. Populate the node with data
    new_node->meter_id = meter_id;
    new_node->fault_type = fault_type;
    
    // 4. Link it to the chain (we insert at the beginning for speed)
    new_node->next = head;
    head = new_node;
}

void Log_Print_All(void) {
    printf("\n=== FINAL SYSTEM FAULT REPORT ===\n");
    FaultNode_t* current = head;
    
    if (current == NULL) {
        printf("No faults recorded. Grid is 100%% stable.\n");
    } else {
        // "Walk" down the linked list until we hit the end (NULL)
        while (current != NULL) {
            printf(" -> Meter %03d experienced: %s\n", current->meter_id, current->fault_type);
            current = current->next; // Move to the next node
        }
    }
    printf("=================================\n\n");
}

void Log_Cleanup(void) {
    FaultNode_t* current = head;
    FaultNode_t* next_node;
    
    // If you malloc(), you MUST free(), otherwise your system will crash over time.
    while (current != NULL) {
        next_node = current->next; // Save the link to the next one
        free(current);             // Destroy the current one
        current = next_node;       // Move forward
    }
    head = NULL;
    printf("[SYSTEM] Linked List destroyed. Zero memory leaks.\n");
}
