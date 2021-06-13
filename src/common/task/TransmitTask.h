/**
 * @file TransmitTask.h
 * @brief Core logic of sending data from app to driver
**/

#ifndef IS_TRANSMIT_TASK_DEFINED
#define IS_TRANSMIT_TASK_DEFINED

#include "m2tp-common/typedef.h"

// Will be called by nextTask
extern void TransmitTask_start();

// Will be called by reset
extern void TransmitTask_stop();

// Will be called by AppInterface
extern void TransmitTask_send(m2tp_channel target, m2tp_byte dataSize, const m2tp_bytes data);

#endif // #ifndef IS_TRANSMIT_TASK_DEFINED
