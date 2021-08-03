/**
 * @file m2tp-member.h
 * @brief Include this if you want the device be a member
**/

// Making sure m2tp-leader and m2tp-member are
// not included in a same file
#ifdef IS_M2TP_LEADER_APP_INTERFACE_DEFINED
#error Conflict with: #include "m2tp-leader.h"
#endif

#ifndef IS_M2TP_MEMBER_APP_INTERFACE_DEFINED
#define IS_M2TP_MEMBER_APP_INTERFACE_DEFINED

#include "./m2tp/interface/app.h"

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

#endif // #ifndef IS_M2TP_MEMBER_APP_INTERFACE_DEFINED
