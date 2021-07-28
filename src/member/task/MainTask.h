/**
 * @file MainTask.h
 * @brief This task handles in and out packet transfer
 * @note Only run when the device is registered to the network
**/

#ifndef IS_MAIN_TASK_DEFINED
#define IS_MAIN_TASK_DEFINED

#include "../../common/packet/Packet.h"

// TransmitBuffer controls pending transmit packet via TaskRouter
extern volatile Packet *MainTask_pendingTransmit;

// Callback came from `TransmitBuffer.finishAsync(...)` via TaskRouter
extern void (*MainTask_onTransmitSuccess)();

// Callback came from `TransmitBuffer.finishAsync(...)` via TaskRouter
extern void (*MainTask_onTransmitFailed)(m2tp_byte errorCode);

// Mandatory function, this will attach private functions
// to TaskRouter then initialize stuffs
extern void MainTask_start();

// Mandatory function, this will detach functions
// from TaskRouter then do cleanups
extern void MainTask_stop();

#endif // #ifndef IS_MAIN_TASK_DEFINED
