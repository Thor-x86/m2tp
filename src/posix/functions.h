/**
 * @file functions.h
 * @brief All functions here are shared among threads
**/

#ifndef IS_M2TP_POSIX_FUNCTIONS_DEFINED
#define IS_M2TP_POSIX_FUNCTIONS_DEFINED

#include "dependencies.h"
#include "typedef.h"
#include "m2tp-posix.h"
#include "variables.h"

// Things to do while waiting for packet being transmitted
extern void onWaitForQueue();

#endif // #ifndef IS_M2TP_POSIX_FUNCTIONS_DEFINED
