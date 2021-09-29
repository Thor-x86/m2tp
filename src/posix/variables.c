/**
 * @file variables.c
 * @brief Implementation of variables.h
**/

#include "variables.h"

ConnectMode connectMode = MODE_OFFLINE;
int descriptor = 0;
int assignedSignalCode = SIGUSR1;
size_t maxPacketSize = 0;
size_t maxFrameSize = 0;
m2tp_posix_ReceiveHook receiveHook = NULL;
m2tp_posix_TransmitHook transmitHook = NULL;
