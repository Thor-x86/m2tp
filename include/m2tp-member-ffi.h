/**
 * @file m2tp-member-ffi.h
 * @brief Allows FFI to set the listeners
 * @see https://en.wikipedia.org/wiki/Foreign_function_interface
 **/

#ifndef IS_M2TP_MEMBER_FFI_DEFINED
#define IS_M2TP_MEMBER_FFI_DEFINED

#include "./m2tp-member.h"
#include "./m2tp/interface/app-ffi.h"

/**
 * @brief Type data for parameter of m2tp_setOnConnected(...) function
 * @param address Current device's new address
 */
typedef void (*m2tp_OnConnected)(m2tp_channel address);

/**
 * @brief Allows FFI to set the on connected listener
 * @param listener Pointer to a function
 */
extern void m2tp_setOnConnected(m2tp_OnConnected listener);

/**
 * @brief Type data for parameter of m2tp_OnDisconnected(...) function
 */
typedef void (*m2tp_OnDisconnected)();

/**
 * @brief Allows FFI to set the on disconnected listener
 * @param listener Pointer to a function
 */
extern void m2tp_setOnDisconnected(m2tp_OnDisconnected listener);

#endif // #ifndef IS_M2TP_MEMBER_FFI_DEFINED
