/**
 * @file Flag.h
 * @brief This file helps us manipulate flags variable
**/

#ifndef IS_FLAG_DEFINED
#define IS_FLAG_DEFINED

#include "m2tp-common/typedef.h"

// Turn a flag at known index to be TRUE
#define Flag_set(target, flagIndex) ((target) |= (1U << (flagIndex)))

// Turn a flag at known index to be FALSE
#define Flag_unset(target, flagIndex) ((target) &= ~(1U << (flagIndex)))

// Check if that flag is TRUE or FALSE
#define Flag_check(target, flagIndex) ((target) & (1U << flagIndex))

#endif // #ifndef IS_FLAG_DEFINED
