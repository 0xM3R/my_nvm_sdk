#include "gpNvm.h"
#include <stdio.h>   // Added for printf
#include <string.h>

// Attribute structure to store data with checksum and backup
typedef union {
    UInt8 u8Val;
    UInt32 u32Val;
    UInt8 arrayVal[GP_NVM_MAX_ATTR_SIZE];  // Array of UInt8
    gpTestData_t structVal;                // Custom struct
} gpNvm_Value;

typedef struct {
    gpNvm_AttrId attrId;
    UInt8 length;
    gpNvm_DataType dataType;  // Type of value stored
    gpNvm_Value value;        // Union for holding different types of data
    UInt32 checksum;          // Checksum for error detection
    gpNvm_Value backup;       // Backup copy for error recovery
} gpNvm_Attribute;

// NVM memory storage
gpNvm_Attribute nvmMemory[GP_NVM_MAX_ATTRS] = {0};

// Function to calculate checksum (simple sum-based checksum)
UInt32 gpNvm_CalculateChecksum(UInt8* data, UInt8 length) {
    UInt32 checksum = 0;
    for (UInt8 i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
}

// Function to set an attribute with checksum and backup
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, gpNvm_DataType dataType, UInt8 length, void* pValue) {
    if (length > GP_NVM_MAX_ATTR_SIZE && dataType != GP_NVM_TYPE_STRUCT) {
        return GP_NVM_FAIL;  // Exceeds size limit
    }

    for (int i = 0; i < GP_NVM_MAX_ATTRS; i++) {
        if (nvmMemory[i].attrId == attrId || nvmMemory[i].attrId == 0) {
            nvmMemory[i].attrId = attrId;
            nvmMemory[i].length = length;
            nvmMemory[i].dataType = dataType;

            // Store the value based on data type
            switch (dataType) {
                case GP_NVM_TYPE_UINT8:
                    nvmMemory[i].value.u8Val = *(UInt8*)pValue;
                    break;
                case GP_NVM_TYPE_UINT32:
                    nvmMemory[i].value.u32Val = *(UInt32*)pValue;
                    break;
                case GP_NVM_TYPE_ARRAY_UINT8:
                    memcpy(nvmMemory[i].value.arrayVal, (UInt8*)pValue, length);
                    break;
                case GP_NVM_TYPE_STRUCT:
                    memcpy(&nvmMemory[i].value.structVal, (gpTestData_t*)pValue, sizeof(gpTestData_t));
                    break;
                default:
                    return GP_NVM_FAIL;
            }

            // Calculate and store the checksum
            UInt8* valuePtr = (UInt8*)&nvmMemory[i].value;
            nvmMemory[i].checksum = gpNvm_CalculateChecksum(valuePtr, length);

            // Store a backup copy for recovery
            memcpy(&nvmMemory[i].backup, &nvmMemory[i].value, sizeof(gpNvm_Value));

            return GP_NVM_SUCCESS;
        }
    }
    return GP_NVM_FAIL;
}

// Function to get an attribute with corruption detection and recovery
gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, gpNvm_DataType* pDataType, UInt8* pLength, void* pValue) {
    for (int i = 0; i < GP_NVM_MAX_ATTRS; i++) {
        if (nvmMemory[i].attrId == attrId) {
            *pLength = nvmMemory[i].length;
            *pDataType = nvmMemory[i].dataType;

            // Calculate the current checksum
            UInt8* valuePtr = (UInt8*)&nvmMemory[i].value;
            UInt32 currentChecksum = gpNvm_CalculateChecksum(valuePtr, nvmMemory[i].length);

            // Check if the data is corrupted
            if (currentChecksum != nvmMemory[i].checksum) {
                // Try to recover from the backup
                printf("Data corruption detected! Attempting to recover...\n");
                memcpy(&nvmMemory[i].value, &nvmMemory[i].backup, sizeof(gpNvm_Value));

                // Recalculate checksum after recovery
                nvmMemory[i].checksum = gpNvm_CalculateChecksum(valuePtr, nvmMemory[i].length);
            }

            // Return the value based on the data type
            switch (nvmMemory[i].dataType) {
                case GP_NVM_TYPE_UINT8:
                    *(UInt8*)pValue = nvmMemory[i].value.u8Val;
                    break;
                case GP_NVM_TYPE_UINT32:
                    *(UInt32*)pValue = nvmMemory[i].value.u32Val;
                    break;
                case GP_NVM_TYPE_ARRAY_UINT8:
                    memcpy((UInt8*)pValue, nvmMemory[i].value.arrayVal, *pLength);
                    break;
                case GP_NVM_TYPE_STRUCT:
                    memcpy((gpTestData_t*)pValue, &nvmMemory[i].value.structVal, sizeof(gpTestData_t));
                    break;
                default:
                    return GP_NVM_FAIL;
            }

            return GP_NVM_SUCCESS;
        }
    }
    return GP_NVM_FAIL;
}
