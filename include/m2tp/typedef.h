/**
 * @file typedef.h
 * @brief Type definition
**/

#ifndef IS_M2TP_TYPEDEF_DEFINED
#define IS_M2TP_TYPEDEF_DEFINED

#include "cpp_compat.h"

// Non-Arduino requires this to use boolean
#include <stdbool.h>

EXTERN_C_START

/**
 * @brief Represents each one byte
 */
typedef unsigned char m2tp_byte;

/**
 * @brief Represents pointer to multiple bytes
 */
typedef m2tp_byte *m2tp_bytes;

/**
 * @brief Standardized how address and topicID formatted
 */
typedef m2tp_byte m2tp_channel;

/**
 * @brief A single byte that represents error code
 * @see https://github.com/Thor-x86/m2tp/blob/dev/include/m2tp/errors.h
 */
typedef m2tp_byte m2tp_error;

EXTERN_C_STOP

// Standardize NULL definition
#ifndef NULL
#define NULL 0
#endif

#endif // #ifndef IS_M2TP_TYPEDEF_DEFINED
