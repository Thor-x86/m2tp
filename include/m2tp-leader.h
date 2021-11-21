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

/**
 * @brief Get registered member's class, with its address known
 * @param deviceAddress Known device address
 * @return Device class, NULL means not assigned
 */
extern const char *m2tp_getMemberDeviceClass(m2tp_channel deviceAddress);

/**
 * @brief Get registered topic name, with its ID known
 * @param topicID Known topic ID
 * @return Topic name, NULL means not assigned
 */
extern const char *m2tp_getTopicName(m2tp_channel topicID);

/**
 * @brief Find member's device address, with its class known
 * @note Always returns first occurence if duplicate found
 * @param deviceClass Known device class
 * @return Device address, 0 means not found
 */
extern m2tp_channel m2tp_findMemberAddress(const char *deviceClass);

/**
 * @brief Find topic ID, with its name known
 * @note Always returns first occurence if duplicate found
 * @param topicName Known topic name
 * @return Topic ID, 0 means not found
 */
extern m2tp_channel m2tp_findTopicID(const char *topicName);

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_LEADER_DEFINED
