/**
 * @file typedef.h
 * @brief Type definition
**/

#ifndef IS_M2TP_TYPEDEF_DEFINED
#define IS_M2TP_TYPEDEF_DEFINED

// Non-Arduino requires this to use boolean
#include <stdbool.h>

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

// Standardize NULL definition
#ifndef NULL
#define NULL 0
#endif

#endif // #ifndef IS_M2TP_TYPEDEF_DEFINED
