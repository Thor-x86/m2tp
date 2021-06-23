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

// Stop current task, then go on to the next task
extern void TaskRouter_nextTask();

// Start timer interrupt that prepared by driver
extern void TaskRouter_startTimeout(unsigned long milliseconds);

// Stop the timer interrupt
extern void TaskRouter_stopTimeout();

// Clear function pointers
extern void TaskRouter_clearFunctions();

// Clear anything related to tasks
extern void TaskRouter_reset();

#endif // #ifndef IS_TASK_ROUTER_DEFINED
