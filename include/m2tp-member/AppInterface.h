/**
 * @file AppInterface.h
 * @brief Allows your program to "talk" with M2TP, via C++
**/

// Making sure m2tp-leader and m2tp-member are
// not included in a same file
#ifdef IS_M2TP_LEADER_APP_INTERFACE_DEFINED
#error Conflict with: #include "m2tp-leader/AppInterface.h"
#endif

#ifndef IS_M2TP_MEMBER_APP_INTERFACE_DEFINED
#define IS_M2TP_MEMBER_APP_INTERFACE_DEFINED

#include "../m2tp-common/base_interface/AppInterface.h"

// TODO: Write stuffs here...

#endif // #ifndef IS_M2TP_MEMBER_APP_INTERFACE_DEFINED
