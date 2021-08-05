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

EXTERN_C_START

// TODO: Write stuffs here...

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_LEADER_DEFINED
