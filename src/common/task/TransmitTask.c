/**
 * @file TransmitTask.c
 * @brief Implementation of TransmitTask.h
**/

#include "TransmitTask.h"
#include "../TaskRouter.h"

//////// Variables /////////////////////////////////////

// TODO: Write stuffs here...

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void TransmitTask_receiveInterrupt(Packet *packet)
{
  // TODO: Write Stuffs here...
}

void TransmitTask_timeoutInterrupt()
{
  // TODO: Write Stuffs here...
}

void TransmitTask_reportReceiveFail(m2tp_byte errorCode)
{
  // TODO: Write Stuffs here...
}

void TransmitTask_reportSendFail(m2tp_byte errorCode)
{
  // TODO: Write Stuffs here...
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void TransmitTask_start()
{
  // Assign functions to router
  TaskRouter_receiveInterrupt = &TransmitTask_receiveInterrupt;
  TaskRouter_timeoutInterrupt = &TransmitTask_timeoutInterrupt;
  TaskRouter_reportReceiveFail = &TransmitTask_reportReceiveFail;
  TaskRouter_reportSendFail = &TransmitTask_reportSendFail;

  // Start timer interrupt
  TaskRouter_startTimeout(1000); // TODO: Adjust this

  // TODO: Write stuffs here...
}

void TransmitTask_stop()
{
  // TODO: Write stuffs here...
}

void TransmitTask_send(m2tp_channel target, m2tp_byte dataSize, const m2tp_bytes data)
{
  // TODO: Write stuffs here...
}

////////////////////////////////////////////////////////
