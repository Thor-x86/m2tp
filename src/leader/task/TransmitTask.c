/**
 * @file TransmitTask.c
 * @brief Implementation of TransmitTask.h
**/

#include "TransmitTask.h"

#include "m2tp-common/commands.h"
#include "m2tp-common/errors.h"
#include "m2tp-member/DriverInterface.h"

#include "../NetworkState.h"
#include "../../common/TaskRouter.h"
#include "../../common/packet/Packet.h"
#include "../../common/packet/content/fail_signal.h"
#include "../../common/packet/content/announcement_quit.h"

//////// Configurations ////////////////////////////////

#define TransmitTask_TIMEOUT 10 // milliseconds

////////////////////////////////////////////////////////

//////// Public Variables //////////////////////////////

Packet *TransmitTask_pendingTransmit = NULL;
void (*TransmitTask_onTransmitSuccess)() = NULL;
void (*TransmitTask_onTransmitFailed)(m2tp_byte errorCode) = NULL;

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

bool TransmitTask_shouldRetransmit(m2tp_byte errorCode)
{
  switch (errorCode)
  {
  case M2TP_ERROR_PACKET_SIZE_MISMATCH:
  case M2TP_ERROR_DATA_SIZE_MISMATCH:
  case M2TP_ERROR_DATA_CORRUPT:
  case M2TP_ERROR_DATA_SIZE_TOO_BIG:
    return true;
  default:
    return false;
  }
}

bool TransmitTask_isPeerTransmit(Packet *packet)
{
  return !(packet->content[1] & 0b10000000U);
}

void TransmitTask_resetPendingTransmit()
{
  TransmitTask_pendingTransmit = NULL;
  TransmitTask_onTransmitSuccess = NULL;
  TransmitTask_onTransmitFailed = NULL;
}

void TransmitTask_receiveInterrupt(Packet *packet)
{
  // Stop timeout
  TaskRouter_stopTimeout();

  switch (packet->command)
  {
  case M2TP_COMMAND_FAIL_SIGNAL:
  {
    // Parse retrieved packet content
    packet_content_FailSignal content;
    packet_content_FailSignal_parse(
        packet->content,
        packet->contentSize,
        &content);

    // Should NOT retransmit?
    if (!TransmitTask_shouldRetransmit(content.errorCode))
    {
      // Error callback
      if (TransmitTask_onTransmitFailed != NULL)
        TransmitTask_onTransmitFailed(content.errorCode);

      // Pending transmit cleanup
      TransmitTask_resetPendingTransmit();
    }
  }
  break;

  case M2TP_COMMAND_SUCCESS_SIGNAL:
  {
    // Success callback
    if (TransmitTask_onTransmitSuccess != NULL)
      TransmitTask_onTransmitSuccess();

    // Pending transmit cleanup
    TransmitTask_resetPendingTransmit();
  }
  break;
  } // switch (packet->command)
}

void TransmitTask_timeoutInterrupt()
{
  // Is pending transmit packet still exist?
  if (TransmitTask_pendingTransmit != NULL)
  {
    // Is pending transmit peer-to-peer?
    if (TransmitTask_isPeerTransmit(TransmitTask_pendingTransmit))
    {
      // Resolve recipient address
      m2tp_channel recipientAddress = TransmitTask_pendingTransmit->content[1];

      // Error callback
      if (TransmitTask_onTransmitFailed != NULL)
        TransmitTask_onTransmitFailed(M2TP_ERROR_ADDRESS_NOT_EXIST);

      // Is targeted address registered?
      if (NetworkState_isAssigned(recipientAddress))
      {
        // Is listener ready?
        if (m2tp_driver_sendListener != NULL)
        {
          // Create packet content
          packet_content_AnnouncementQuit content;
          content.address = recipientAddress;

          // Serialize packet content
          m2tp_byte serializedContent[255];
          m2tp_byte serializedContentSize = 0;
          packet_content_AnnouncementQuit_serialize(&content, serializedContent, &serializedContentSize);

          // Send member quit announcement
          m2tp_driver_sendListener(M2TP_COMMAND_ANNOUNCEMENT_QUIT, serializedContentSize, serializedContent);
        }

        // Unregister recipient address
        NetworkState_unassign(recipientAddress);
      }
    }
  }

  // Reset pending transmit packet
  TransmitTask_resetPendingTransmit();

  // Stop current task, then go to the next one
  TaskRouter_nextTask();
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void TransmitTask_start()
{
  // Has pending transmit packet?
  if (TransmitTask_pendingTransmit != NULL)
  {
    // Send the packet
    if (m2tp_driver_sendListener != NULL)
      m2tp_driver_sendListener(
          TransmitTask_pendingTransmit->command,
          TransmitTask_pendingTransmit->contentSize,
          TransmitTask_pendingTransmit->content);

    // Is peer-to-peer transmit?
    if (TransmitTask_isPeerTransmit(TransmitTask_pendingTransmit))
    {
      // Assign functions to router
      TaskRouter_receiveInterrupt = &TransmitTask_receiveInterrupt;
      TaskRouter_timeoutInterrupt = &TransmitTask_timeoutInterrupt;

      // Start timer interrupt
      TaskRouter_startTimeout(TransmitTask_TIMEOUT);
    }
  }

  // ...if not, then skip to the next task
  else
    TaskRouter_nextTask();
}

void TransmitTask_stop()
{
  // Stop timeout
  TaskRouter_stopTimeout();

  // Detach functions
  TaskRouter_receiveInterrupt = NULL;
  TaskRouter_timeoutInterrupt = NULL;
}

////////////////////////////////////////////////////////
