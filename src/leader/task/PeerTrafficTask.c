/**
 * @file PeerTrafficTask.c
 * @brief Implementation of PeerTrafficTask.h
**/

#include "PeerTrafficTask.h"
#include "../../common/TaskRouter.h"

//////// Variables /////////////////////////////////////

// TODO: Write stuffs here...

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void PeerTrafficTask_receiveInterrupt(Packet *packet)
{
  // TODO: Write Stuffs here...
}

void PeerTrafficTask_timeoutInterrupt()
{
  // TODO: Write Stuffs here...
}

void PeerTrafficTask_reportReceiveFail(m2tp_byte errorCode, m2tp_channel where)
{
  // TODO: Write Stuffs here...
}

void PeerTrafficTask_reportSendFail(m2tp_byte errorCode, m2tp_channel where)
{
  // TODO: Write Stuffs here...
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void PeerTrafficTask_start()
{
  // Assign functions to router
  TaskRouter_receiveInterrupt = &PeerTrafficTask_receiveInterrupt;
  TaskRouter_timeoutInterrupt = &PeerTrafficTask_timeoutInterrupt;
  TaskRouter_reportReceiveFail = &PeerTrafficTask_reportReceiveFail;
  TaskRouter_reportSendFail = &PeerTrafficTask_reportSendFail;

  // Start timer interrupt
  TaskRouter_startTimeout(1000); // TODO: Adjust this

  // TODO: Write stuffs here...
}

void PeerTrafficTask_stop()
{
  // TODO: Write stuffs here...
}

////////////////////////////////////////////////////////
