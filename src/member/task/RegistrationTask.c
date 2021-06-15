/**
 * @file RegistrationTask.c
 * @brief Implementation of RegistrationTask.h
**/

#include "RegistrationTask.h"
#include "../../common/TaskRouter.h"

//////// Variables /////////////////////////////////////

// TODO: Write stuffs here...

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void RegistrationTask_receiveInterrupt(Packet *packet)
{
  // TODO: Write Stuffs here...
}

void RegistrationTask_timeoutInterrupt()
{
  // TODO: Write Stuffs here...
}

void RegistrationTask_reportReceiveFail(m2tp_byte errorCode)
{
  // TODO: Write Stuffs here...
}

void RegistrationTask_reportSendFail(m2tp_byte errorCode)
{
  // TODO: Write Stuffs here...
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void RegistrationTask_start()
{
  // Assign functions to router
  TaskRouter_receiveInterrupt = &RegistrationTask_receiveInterrupt;
  TaskRouter_timeoutInterrupt = &RegistrationTask_timeoutInterrupt;
  TaskRouter_reportReceiveFail = &RegistrationTask_reportReceiveFail;
  TaskRouter_reportSendFail = &RegistrationTask_reportSendFail;

  // Start timer interrupt
  TaskRouter_startTimeout(1000); // TODO: Adjust this

  // TODO: Write stuffs here...
}

void RegistrationTask_stop()
{
  // TODO: Write stuffs here...
}

////////////////////////////////////////////////////////
