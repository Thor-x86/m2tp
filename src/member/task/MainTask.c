/**
 * @file MainTask.c
 * @brief Implementation of MainTask.h
**/

#include "MainTask.h"

#include "m2tp-common/commands.h"
#include "m2tp-common/errors.h"
#include "m2tp-member/AppInterface.h"
#include "m2tp-member/DriverInterface.h"

#include "../../common/Flag.h"
#include "../../common/TaskRouter.h"
#include "../../common/DeviceState.h"
#include "../../common/packet/content/channel_turn_signal.h"
#include "../../common/packet/content/fail_signal.h"
#include "../../common/packet/content/transmit.h"
#include "../../common/packet/content/announcement_join.h"
#include "../../common/packet/content/announcement_quit.h"

//////// Configurations ////////////////////////////////

#define MainTask_TIMEOUT_IDLE 1000   // milliseconds
#define MainTask_TIMEOUT_RESPONSE 10 // milliseconds

////////////////////////////////////////////////////////

//////// Available Flags ///////////////////////////////

#define MainTask_FLAG_SENT 0
// #define MainTask_FLAG_EXAMPLE 1

// NOTE: Use flag instead of boolean,
//       It saves a lot of memory!

////////////////////////////////////////////////////////

//////// Public Variables //////////////////////////////

Packet *MainTask_pendingTransmit = NULL;
void (*MainTask_onTransmitSuccess)() = NULL;
void (*MainTask_onTransmitFailed)(m2tp_byte errorCode) = NULL;

////////////////////////////////////////////////////////

//////// Private Variables /////////////////////////////

m2tp_byte MainTask_flags = 0;

////////////////////////////////////////////////////////

//////// Syntactic Sugars //////////////////////////////

#define hasPendingTransmit MainTask_pendingTransmit != NULL

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

bool MainTask_shouldRetransmit(m2tp_byte errorCode)
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

void MainTask_resetPendingTransmit()
{
  MainTask_pendingTransmit = NULL;
  MainTask_onTransmitSuccess = NULL;
  MainTask_onTransmitFailed = NULL;
}

void MainTask_receiveInterrupt(Packet *packet)
{
  // Stop timeout
  TaskRouter_stopTimeout();

  switch (packet->command)
  {
  case M2TP_COMMAND_CHANNEL_TURN_SIGNAL:
  {
    // Parse packet content
    packet_content_ChannelTurnSignal content;
    packet_content_ChannelTurnSignal_parse(
        packet->content,
        packet->contentSize,
        &content);

    // Is my turn?
    if (content.channel == DeviceState_assignedAddress)
    {
      // Has pending data?
      if (hasPendingTransmit)
      {
        // Send the data
        if (m2tp_driver_sendListener != NULL)
          m2tp_driver_sendListener(
              MainTask_pendingTransmit->command,
              MainTask_pendingTransmit->contentSize,
              MainTask_pendingTransmit->content);

        // Set sent flag
        Flag_set(MainTask_flags, MainTask_FLAG_SENT);
      }
      else
      {
        // Send success signal, represents skip transmit
        if (m2tp_driver_sendListener != NULL)
          m2tp_driver_sendListener(M2TP_COMMAND_SUCCESS_SIGNAL, 0, NULL);
      }
    }
  }
  break;

  case M2TP_COMMAND_FAIL_SIGNAL:
    // Sent flag enabled?
    if (Flag_check(MainTask_flags, MainTask_FLAG_SENT))
    {
      // Parse packet content
      packet_content_FailSignal content;
      packet_content_FailSignal_parse(
          packet->content,
          packet->contentSize,
          &content);

      // Unset sent flag
      Flag_unset(MainTask_flags, MainTask_FLAG_SENT);

      // Should NOT retransmit?
      if (!MainTask_shouldRetransmit(content.errorCode))
      {
        // Call error callback
        if (MainTask_onTransmitFailed != NULL)
          MainTask_onTransmitFailed(content.errorCode);

        // Clear pending data transmit
        MainTask_resetPendingTransmit();
      }
    }
    break;

  case M2TP_COMMAND_SUCCESS_SIGNAL:
    // Sent flag enabled?
    if (Flag_check(MainTask_flags, MainTask_FLAG_SENT))
    {
      // Unset sent flag
      Flag_unset(MainTask_flags, MainTask_FLAG_SENT);

      // Success callback
      if (MainTask_onTransmitSuccess != NULL)
        MainTask_onTransmitSuccess();

      // Clear pending data transmit
      MainTask_resetPendingTransmit();
    }
    break;

  case M2TP_COMMAND_TRANSMIT:
  {
    // Parse packet content
    packet_content_Transmit content;
    packet_content_Transmit_parse(
        packet->content,
        packet->contentSize,
        &content);

    // Is this broadcast?
    if (content.target & 0b10000000)
    {
      // Is that packet for my subscribed topic?
      if (DeviceState_isTopicAssigned(content.target))
      {
        // Forward data to app
        DeviceState_callTopicListener(content.target, content.dataSize, content.data);
      }
    }

    // ...or is this peer-to-peer?
    else
    {
      // Is that packet for me?
      if (content.target == DeviceState_assignedAddress)
      {
        // Forward data to app
        if (m2tp_receivedListener != NULL)
          m2tp_receivedListener(content.source, content.dataSize, content.data);

        // Send success signal
        if (m2tp_driver_sendListener != NULL)
          m2tp_driver_sendListener(M2TP_COMMAND_SUCCESS_SIGNAL, 0, NULL);
      }
    }
  }
  break;

  case M2TP_COMMAND_ANNOUNCEMENT_JOIN:
  {
    // Parse packet content
    char deviceClass[packet->contentSize + 1];
    packet_content_AnnouncementJoin content;
    content.deviceClass = deviceClass;
    packet_content_AnnouncementJoin_parse(packet->content, packet->contentSize, &content);

    // Is that my address?
    if (content.address == DeviceState_assignedAddress)
    {
      // Make sure device class matches with current device
      bool isClassMatch = true;
      for (m2tp_byte i = 0; i < 254; i++)
      {
        if (deviceClass[i] != DeviceState_deviceClass[i])
        {
          isClassMatch = false;
          break;
        }
        else if (deviceClass[i] == '\0' || DeviceState_deviceClass[i] == '\0')
          break;
      }

      // If device class isn't match, then current device must re-register
      if (!isClassMatch)
      {
        TaskRouter_nextTask();
        return;
      }
    }

    // Forward to app
    if (m2tp_onNewMemberListener != NULL)
      m2tp_onNewMemberListener(deviceClass, content.address);
  }
  break;

  case M2TP_COMMAND_ANNOUNCEMENT_QUIT:
  {
    // Parse packet content
    packet_content_AnnouncementQuit content;
    packet_content_AnnouncementQuit_parse(packet->content, packet->contentSize, &content);

    // am I quit?
    if (content.address == DeviceState_assignedAddress)
    {
      // If yes, then switch to registration task
      TaskRouter_nextTask();
      return;
    }

    // ...otherwise forward announcement to app
    m2tp_onAnotherMemberQuitListener(content.address);
  }
  break;
  } // switch (packet->command)

  // Start timeout
  if (Flag_check(MainTask_flags, MainTask_FLAG_SENT))
    TaskRouter_startTimeout(MainTask_TIMEOUT_RESPONSE);
  else
    TaskRouter_startTimeout(MainTask_TIMEOUT_IDLE);
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void MainTask_start()
{
  // Stop main task if still not registered yet
  if (!DeviceState_isReady())
  {
    TaskRouter_nextTask();
    return;
  }

  // Assign functions to router
  TaskRouter_receiveInterrupt = &MainTask_receiveInterrupt;
  TaskRouter_timeoutInterrupt = &MainTask_stop; // Stop task on timeout

  // Start timer interrupt
  TaskRouter_startTimeout(MainTask_TIMEOUT_IDLE);

  // Notify the app
  if (m2tp_onConnected != NULL)
    m2tp_onConnected(DeviceState_assignedAddress);
}

void MainTask_stop()
{
  // Stop timeout
  TaskRouter_stopTimeout();

  // Detach functions
  TaskRouter_receiveInterrupt = NULL;
  TaskRouter_timeoutInterrupt = NULL;

  // Pending transmit cleanup
  MainTask_resetPendingTransmit();

  // Reset private variables
  MainTask_flags = 0;

  // Notify the app
  if (m2tp_onDisconnected != NULL)
    m2tp_onDisconnected();

  // Reset device address
  DeviceState_assignedAddress = NULL;
}

////////////////////////////////////////////////////////
