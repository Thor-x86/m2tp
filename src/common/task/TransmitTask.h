/**
 * @file TransmitTask.h
 * @brief Core logic of sending data from app to driver
**/

#ifndef IS_TRANSMIT_TASK_DEFINED
#define IS_TRANSMIT_TASK_DEFINED

#include <stdbool.h>

#include "../packet/Packet.h"

// Will be called by nextTask
extern void TransmitTask_start();

// Will be called by reset
extern void TransmitTask_stop();

// Will be called by TransmitBuffer
extern void TransmitTask_send(Packet *packet);

// Will be called by TransmitBuffer
extern bool TransmitTask_hasPendingPacket();

// Expose private variables to unit test framework
#ifdef GOOGLETEST_INCLUDE_GTEST_GTEST_H_
extern Packet *pendingPacket;
#endif

#endif // #ifndef IS_TRANSMIT_TASK_DEFINED
