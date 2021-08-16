/**
 * @file variables.h
 * @brief All variables here are shared among threads
**/

#ifndef IS_M2TP_POSIX_VARIABLES_DEFINED
#define IS_M2TP_POSIX_VARIABLES_DEFINED

#include "dependencies.h"
#include "typedef.h"

// Possible values:
// MODE_OFFLINE, MODE_FILE, MODE_SOCKET
extern ConnectMode connectMode;

// Can be either as file descriptor or socket descriptor
extern int descriptor;

// UNIX Signal that used for pausing main thread
extern int assignedSignalCode;

#endif // #ifndef IS_M2TP_POSIX_VARIABLES_DEFINED
