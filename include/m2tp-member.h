/**
 * @file m2tp-member.h
 * @brief Include this if you want the device be a member
**/

// Making sure m2tp-leader and m2tp-member are
// not included in a same file
#ifdef IS_M2TP_LEADER_DEFINED
#error Conflict with: #include "m2tp-leader.h"
#endif

#ifndef IS_M2TP_MEMBER_DEFINED
#define IS_M2TP_MEMBER_DEFINED

#include "./m2tp/interface/app.h"

// Automatically include driver glue for POSIX machine
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include "./m2tp-posix.h"
#endif

EXTERN_C_START

/**
 * @brief Pointer to a function, will be called when successfully connected to the network
 * @param address Current device's new address
 */
extern void (*m2tp_onConnected)(m2tp_channel address);

/**
 * @brief Pointer to a function, will be called when disconnected from network
 */
extern void (*m2tp_onDisconnected)();

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_MEMBER_DEFINED
