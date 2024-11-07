// common.h

/*
 * =========================================
 * SECTION: Common Data Types
 * =========================================
 */

/// @brief Union to represent different types of elements.
typedef union elem elem_t;

/// @brief Union representing an element that can hold various data types.
union elem{
    int intValue;           // Represents an integer value
    unsigned int uintValue; // Represents an unsigned integer value
    bool boolValue;         // Represents a boolean value
    float floatValue;       // Represents a floating-point value
    void *ptrValue;         // Represents a generic pointer
    // Add other types as needed
};
