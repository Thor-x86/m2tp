/**
 * @file TaskRouter.h
 * @brief Forward interrupt calls to current working task
**/

#ifndef IS_TASK_ROUTER_DEFINED
#define IS_TASK_ROUTER_DEFINED

#include "m2tp/typedef.h"
#include "m2tp/interface/app.h"
#include "packet/Packet.h"

// Which task currently in?
// If selects nothing, then it's 0
extern volatile m2tp_byte TaskRouter_currentTask;

// Pointer to a function, this will be called when
// receive interrupt from network hardware via its driver
extern void (*TaskRouter_receiveInterrupt)(Packet *packet);

// Pointer to a function, this will be called when
// interrupt triggered from timer hardware
extern void (*TaskRouter_timeoutInterrupt)();

// Initialize the task router, called when driver just ready
extern void TaskRouter_start();

// Stop current task then go on to the next task, called from any tasks to end themselves
extern void TaskRouter_nextTask();

// Stop current task then reset all, called when the hardware stopped working
extern void TaskRouter_stop();

// Send packet to a task depends on command value inside that packet
extern void TaskRouter_sendPacket(Packet *packet);

// Like `sendPacket` but has callbacks to know if it fail or success
extern void TaskRouter_sendPacketAsync(
    Packet *packet,
    m2tp_OnSuccessCallback successCallback,
    m2tp_OnErrorCallback failedCallback);

// Check if there is a packet with "transmit" content type
// that didn't send properly yet, required for TransmitBuffer
extern volatile bool TaskRouter_hasPendingData();

// Both createTopic() and subscribe() will call this
extern void TaskRouter_assignTopic(
    char *topicName,
    m2tp_OnRegisteredCallback registeredCallback,
    m2tp_TopicListener listener,
    m2tp_OnErrorCallback failedCallback);

// Start timer interrupt that prepared by driver
extern void TaskRouter_startTimeout(unsigned long milliseconds);

// Stop the timer interrupt
extern void TaskRouter_stopTimeout();

#endif // #ifndef IS_TASK_ROUTER_DEFINED
