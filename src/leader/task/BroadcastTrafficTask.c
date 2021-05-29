/**
 * @file BroadcastTrafficTask.c
 * @brief Implementation of BroadcastTrafficTask.h
**/

#include "BroadcastTrafficTask.h"
#include "../../common/TaskRouter.h"

//////// Variables /////////////////////////////////////

// TODO: Write stuffs here...

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void BroadcastTrafficTask_receiveInterrupt(Packet *packet)
{
  // TODO: Write Stuffs here...
}

void BroadcastTrafficTask_timeoutInterrupt()
{
  // TODO: Write Stuffs here...
}

void BroadcastTrafficTask_reportReceiveFail(m2tp_byte errorCode, m2tp_channel where)
{
  // TODO: Write Stuffs here...
}

void BroadcastTrafficTask_reportSendFail(m2tp_byte errorCode, m2tp_channel where)
{
  // TODO: Write Stuffs here...
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void BroadcastTrafficTask_start()
{
  // Assign functions to router
  TaskRouter_receiveInterrupt = &BroadcastTrafficTask_receiveInterrupt;
  TaskRouter_timeoutInterrupt = &BroadcastTrafficTask_timeoutInterrupt;
  TaskRouter_reportReceiveFail = &BroadcastTrafficTask_reportReceiveFail;
  TaskRouter_reportSendFail = &BroadcastTrafficTask_reportSendFail;

  // Start timer interrupt
  TaskRouter_startTimeout(1000); // TODO: Adjust this

  // TODO: Write stuffs here...
}

void BroadcastTrafficTask_stop()
{
  // TODO: Write stuffs here...
}

////////////////////////////////////////////////////////
