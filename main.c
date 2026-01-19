#include <stdio.h>
#include "include/cphysics.h"

int main(void) {
    printf("=== CPhysics Engine Test Suite ===\n\n");

    // Test 1: Create Entity and test show_entity_details
    printf("[Test 1] Testing show_entity_details function...\n");
    double pos[] = {1.0, 2.0, 3.0};
    double vel[] = {0.5, -0.3, 0.8};
    double acc[] = {0.1, 0.2, -0.1};
    
    Entity test_entity = new_entity("Test Object", 10.0, 5.0, pos, vel, acc, 0.7, true, false);
    
    // Set additional properties for testing
    test_entity.quaternion[0] = 0.7071; // w
    test_entity.quaternion[1] = 0.0;    // x
    test_entity.quaternion[2] = 0.7071; // y
    test_entity.quaternion[3] = 0.0;    // z
    
    test_entity.angular_velocity[0] = 0.1;
    test_entity.angular_velocity[1] = 0.2;
    test_entity.angular_velocity[2] = 0.3;
    
    test_entity.angular_acceleration[0] = 0.01;
    test_entity.angular_acceleration[1] = 0.02;
    test_entity.angular_acceleration[2] = 0.03;
    
    test_entity.moment_of_inertia = 2.5;
    
    // Test the show_entity_details function
    char* entity_address = show_entity_details(&test_entity);
    printf("\nReturned address: %p\n", (void*)entity_address);
    printf("Original address: %p\n", (void*)&test_entity);
    
    // Verify the addresses match
    if (entity_address == (char*)&test_entity) {
        printf("✓ Address verification: PASSED\n");
    } else {
        printf("✗ Address verification: FAILED\n");
    }
    
    printf("\n[Test 2] Testing with static entity...\n");
    double static_pos[] = {0.0, 0.0, 0.0};
    double static_vel[] = {0.0, 0.0, 0.0};
    double static_acc[] = {0.0, 0.0, 0.0};
    
    Entity static_entity = new_entity("Static Object", 100.0, 0.0, static_pos, static_vel, static_acc, 0.5, true, true);
    show_entity_details(&static_entity);
    
    printf("\n=== All tests completed ===\n");
    
    return 0;
}