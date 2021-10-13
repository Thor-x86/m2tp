/**
 * @file m2tp-leader.h
 * @brief Include this if you want the device be a leader
**/

// Making sure m2tp-leader and m2tp-member are
// not included in a same file
#ifdef IS_M2TP_MEMBER_DEFINED
#error Conflict with: #include "m2tp-member.h"
#endif

#ifndef IS_M2TP_LEADER_DEFINED
#define IS_M2TP_LEADER_DEFINED

#include "./m2tp/interface/app.h"

// Automatically include driver glue for POSIX machine
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include "./m2tp-posix.h"
#endif

EXTERN_C_START

/**
 * @brief Useful if the network interface doesn't support broadcasting
 * @param value Insert `true` to enable
 */
extern void m2tp_enableEchoTransmit(bool value);

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_LEADER_DEFINED
