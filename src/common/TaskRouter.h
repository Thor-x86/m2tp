/**
 * @file TaskRouter.h
 * @brief Forward interrupt calls to current working task
**/

#ifndef IS_TASK_ROUTER_DEFINED
#define IS_TASK_ROUTER_DEFINED

#include "m2tp-common/typedef.h"
#include "packet/Packet.h"

// Which task currently in?
// If selects nothing, then it's NULL
extern m2tp_byte TaskRouter_currentTask;

// Pointer to receiveInterrupt function, depends on current task
extern void (*TaskRouter_receiveInterrupt)(Packet *packet);

// Pointer to timeoutInterrupt function, depends on current task
extern void (*TaskRouter_timeoutInterrupt)();

// Pointer to reportReceiveFail function, depends on current task
extern void (*TaskRouter_reportReceiveFail)(m2tp_byte errorCode);

// Pointer to reportSendFail function, depends on current task
extern void (*TaskRouter_reportSendFail)(m2tp_byte errorCode);

// Pointer to endTask function, depends on current task
extern void (*TaskRouter_endTask)();

// Initialize the task router, called when driver just ready
extern void TaskRouter_start();

// Stop current task then go on to the next task, called from any tasks to end themselves
extern void TaskRouter_nextTask();

// Stop current task then reset all, called when the hardware stopped working
extern void TaskRouter_stop();

// Send packet to a task depends on command value inside that packet
extern void TaskRouter_sendPacket(Packet *packet);

// Check if there is a packet with "transmit" content type
// that didn't send properly yet, required for TransmitBuffer
extern bool TaskRouter_hasPendingData();

// Start timer interrupt that prepared by driver
extern void TaskRouter_startTimeout(unsigned long milliseconds);

// Stop the timer interrupt
extern void TaskRouter_stopTimeout();

#endif // #ifndef IS_TASK_ROUTER_DEFINED
