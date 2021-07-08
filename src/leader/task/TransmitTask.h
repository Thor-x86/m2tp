/**
 * @file TransmitTask.h
 * @brief Transmit leader's data
**/

#ifndef IS_TRANSMIT_TASK_DEFINED
#define IS_TRANSMIT_TASK_DEFINED

#include "../../common/packet/Packet.h"

// TransmitBuffer controls pending transmit packet via TaskRouter
extern Packet *TransmitTask_pendingTransmit;

// Callback came from `TransmitBuffer.finishAsync(...)` via TaskRouter
extern void (*TransmitTask_onTransmitSuccess)();

// Callback came from `TransmitBuffer.finishAsync(...)` via TaskRouter
extern void (*TransmitTask_onTransmitFailed)(m2tp_byte errorCode);

// Mandatory function, this will attach private functions
// to TaskRouter then initialize stuffs
extern void TransmitTask_start();

// Mandatory function, this will detach functions
// from TaskRouter then do cleanups
extern void TransmitTask_stop();

#endif // #ifndef IS_TRANSMIT_TASK_DEFINED
