#include <stdio.h>
#include <stdint.h>
#include "gpNvm.h"

void runTests() {
    printf("Running tests...\n");

    UInt8 length;
    gpNvm_DataType dataType;

    // Test 1: UInt8 Value
    printf("Test 1: Setting UInt8 value\n");
    UInt8 u8Value = 0x12;
    if (gpNvm_SetAttribute(1, GP_NVM_TYPE_UINT8, sizeof(u8Value), &u8Value) == GP_NVM_SUCCESS) {
        UInt8 retrievedU8;
        if (gpNvm_GetAttribute(1, &dataType, &length, &retrievedU8) == GP_NVM_SUCCESS) {
            printf("Test 1: UInt8 Value - Passed (retrieved value: 0x%02X)\n", retrievedU8);
        } else {
            printf("Test 1: UInt8 Value - Failed on GetAttribute\n");
        }
    } else {
        printf("Test 1: UInt8 Value - Failed on SetAttribute\n");
    }

    // Test 2: UInt32 Value
    printf("Test 2: Setting UInt32 value\n");
    UInt32 u32Value = 0x12345678;
    if (gpNvm_SetAttribute(2, GP_NVM_TYPE_UINT32, sizeof(u32Value), &u32Value) == GP_NVM_SUCCESS) {
        UInt32 retrievedU32;
        if (gpNvm_GetAttribute(2, &dataType, &length, &retrievedU32) == GP_NVM_SUCCESS) {
            printf("Test 2: UInt32 Value - Passed (retrieved value: 0x%08X)\n", retrievedU32);
        } else {
            printf("Test 2: UInt32 Value - Failed on GetAttribute\n");
        }
    } else {
        printf("Test 2: UInt32 Value - Failed on SetAttribute\n");
    }

    // Test 3: Array of UInt8
    printf("Test 3: Setting an array of UInt8\n");
    UInt8 arrayValue[] = {0x01, 0x02, 0x03};
    if (gpNvm_SetAttribute(3, GP_NVM_TYPE_ARRAY_UINT8, sizeof(arrayValue), arrayValue) == GP_NVM_SUCCESS) {
        UInt8 retrievedArray[GP_NVM_MAX_ATTR_SIZE];
        if (gpNvm_GetAttribute(3, &dataType, &length, retrievedArray) == GP_NVM_SUCCESS) {
            printf("Test 3: Array of UInt8 - Passed (retrieved array: ");
            for (int i = 0; i < length; i++) {
                printf("0x%02X ", retrievedArray[i]);
            }
            printf(")\n");
        } else {
            printf("Test 3: Array of UInt8 - Failed on GetAttribute\n");
        }
    } else {
        printf("Test 3: Array of UInt8 - Failed on SetAttribute\n");
    }

    // Test 4: Struct Value
    printf("Test 4: Setting a struct value\n");
    gpTestData_t testData = { .id = 0x01, .options = 0x1234ABCD, .length = 4, .data = {0x10, 0x20, 0x30, 0x40} };
    if (gpNvm_SetAttribute(4, GP_NVM_TYPE_STRUCT, sizeof(testData), &testData) == GP_NVM_SUCCESS) {
        gpTestData_t retrievedStruct;
        if (gpNvm_GetAttribute(4, &dataType, &length, &retrievedStruct) == GP_NVM_SUCCESS) {
            printf("Test 4: Struct Value - Passed (retrieved struct id=0x%02X, options=0x%08X, length=%d, data=", 
                   retrievedStruct.id, retrievedStruct.options, retrievedStruct.length);
            for (int i = 0; i < retrievedStruct.length; i++) {
                printf("0x%02X ", retrievedStruct.data[i]);
            }
            printf(")\n");
        } else {
            printf("Test 4: Struct Value - Failed on GetAttribute\n");
        }
    } else {
        printf("Test 4: Struct Value - Failed on SetAttribute\n");
    }
}

int main() {
    runTests();
    return 0;
}
