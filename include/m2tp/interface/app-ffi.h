/**
 * @file app-ffi.h
 * @brief Allows FFI to set the listeners
 * @see https://en.wikipedia.org/wiki/Foreign_function_interface
**/

#ifndef IS_M2TP_INTERFACE_APP_FFI_DEFINED
#define IS_M2TP_INTERFACE_APP_FFI_DEFINED

#include "../typedef.h"
#include "../cpp_compat.h"

EXTERN_C_START

/**
 * @brief Type data for parameter of m2tp_setReceivedListener(...) function
 * @param sourceAddress Identity of the sender
 * @param dataSize In bytes, at most 253 bytes
 * @param data Received raw bytes of data
 */
typedef void (*m2tp_ReceivedListener)(m2tp_channel sourceAddress, m2tp_byte dataSize, const m2tp_bytes data);

/**
 * @brief Allows FFI to set the received listener
 * @param listener Pointer to a function
 */
extern void m2tp_setReceivedListener(m2tp_ReceivedListener listener);

/**
 * @brief Type data for parameter of m2tp_setNewMemberListener(...) function
 * @param memberClass Type of device, came from that device's driver interface
 * @param assignedAddress New member's address
 */
typedef void (*m2tp_NewMemberListener)(char *memberClass, m2tp_channel assignedAddress);

/**
 * @brief Allows FFI to set the new member listener
 * @param listener Pointer to a function
 */
extern void m2tp_setNewMemberListener(m2tp_NewMemberListener listener);

/**
 * @brief Type data for parameter of m2tp_setAnotherMemberQuitListener(...) function
 * @param assignedAddress That member's address
 */
typedef void (*m2tp_AnotherMemberQuitListener)(m2tp_channel assignedAddress);

/**
 * @brief Allows FFI to set the new member listener
 * @param listener Pointer to a function
 */
extern void m2tp_setAnotherMemberQuitListener(m2tp_AnotherMemberQuitListener listener);

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_INTERFACE_APP_FFI_DEFINED
