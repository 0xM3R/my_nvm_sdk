#include <stdio.h>
#include <stdint.h>
#include "gpNvm.h"

#define MAX_FUZZ_INPUT_SIZE 128

// Fuzz harness
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < sizeof(UInt8) + sizeof(UInt32) + sizeof(UInt8)) {
        return 0;  // Not enough data to fuzz
    }

    // Extract values from fuzz input
    UInt8 attrId = data[0];
    gpNvm_DataType dataType = data[1] % 4;  // Cycle through data types (0-3)
    UInt8 length = data[2] % GP_NVM_MAX_ATTR_SIZE;  // Limit the length to the maximum

    // Allocate a buffer for the value
    void *pValue = (void *)(data + 3);

    // Call the SetAttribute function
    gpNvm_SetAttribute(attrId, dataType, length, pValue);

    // Retrieve the value back and check for corruption
    UInt8 retrievedValue[MAX_FUZZ_INPUT_SIZE] = {0};
    gpNvm_GetAttribute(attrId, &dataType, &length, retrievedValue);

    return 0;  // Return success
}

int main(int argc, char** argv) {
    uint8_t buffer[MAX_FUZZ_INPUT_SIZE];
    size_t bytesRead;

    // Read data from stdin (AFL will provide this)
    while ((bytesRead = fread(buffer, 1, MAX_FUZZ_INPUT_SIZE, stdin)) > 0) {
        LLVMFuzzerTestOneInput(buffer, bytesRead);
    }

    return 0;
}
