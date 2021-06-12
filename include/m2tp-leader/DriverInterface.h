/**
 * @file DriverInterface.h
 * @brief Allows network driver to "talk" with M2TP
**/

// Making sure m2tp-leader and m2tp-member are
// not included in a same file
#ifdef IS_M2TP_MEMBER_DRIVER_INTERFACE_DEFINED
#error Conflict with: #include "m2tp-member/DriverInterface.h"
#endif

#ifndef IS_M2TP_LEADER_DRIVER_INTERFACE_DEFINED
#define IS_M2TP_LEADER_DRIVER_INTERFACE_DEFINED

#include "../m2tp-common/base_interface/DriverInterface.h"

// TODO: Write stuffs here...

#endif // #ifndef IS_M2TP_LEADER_DRIVER_INTERFACE_DEFINED
