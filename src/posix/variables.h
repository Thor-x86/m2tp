/**
 * @file variables.h
 * @brief All variables here are shared among threads
**/

#ifndef IS_M2TP_POSIX_VARIABLES_DEFINED
#define IS_M2TP_POSIX_VARIABLES_DEFINED

#include "dependencies.h"
#include "typedef.h"
#include "m2tp-posix.h"

// Possible values:
// MODE_OFFLINE, MODE_FILE, MODE_SOCKET
extern ConnectMode connectMode;

// Can be either as file descriptor or socket descriptor
extern int descriptor;

// UNIX Signal that used for pausing main thread
extern int assignedSignalCode;

// NULL means receive data as-is
extern m2tp_posix_ReceiveHook receiveHook;

// NULL means transmit data as-is
extern m2tp_posix_TransmitHook transmitHook;

// Maximum network's frame size, only used when hooks are available
extern size_t maxFrameSize;

#endif // #ifndef IS_M2TP_POSIX_VARIABLES_DEFINED
