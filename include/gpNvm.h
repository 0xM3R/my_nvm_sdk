#ifndef GP_NVM_H
#define GP_NVM_H

#include <stdint.h>

// Type definitions
typedef uint8_t UInt8;
typedef uint32_t UInt32;
typedef UInt8 gpNvm_AttrId;
typedef UInt8 gpNvm_Result;

// Result codes
#define GP_NVM_SUCCESS 0
#define GP_NVM_FAIL    1
#define GP_NVM_CORRUPTED 2  // New result code for corruption detection

// Max length of array in struct
#define MAX_LENGTH 20

// Attribute storage limits
#define GP_NVM_MAX_ATTRS 10
#define GP_NVM_MAX_ATTR_SIZE 64

// Supported data types
typedef enum {
    GP_NVM_TYPE_UINT8,
    GP_NVM_TYPE_UINT32,
    GP_NVM_TYPE_ARRAY_UINT8,
    GP_NVM_TYPE_STRUCT
} gpNvm_DataType;

// Custom struct definition
typedef struct {
    UInt8 id;
    UInt32 options;
    UInt8 length;
    UInt8 data[MAX_LENGTH];
} gpTestData_t;

// Public API functions
gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId, gpNvm_DataType dataType, UInt8 length, void* pValue);
gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId, gpNvm_DataType* pDataType, UInt8* pLength, void* pValue);

// Checksum utility
UInt32 gpNvm_CalculateChecksum(UInt8* data, UInt8 length);

#endif // GP_NVM_H
