/**
 * @file TrafficTask.c
 * @brief Implementation of TrafficTask.h
**/

#include "TrafficTask.h"

#include "m2tp/commands.h"
#include "m2tp/errors.h"
#include "m2tp-leader.h"
#include "m2tp/interface/driver.h"

#include "../NetworkState.h"
#include "../../common/DeviceState.h"
#include "../../common/TaskRouter.h"
#include "../../common/packet/Packet.h"
#include "../../common/packet/content/channel_turn_signal.h"
#include "../../common/packet/content/transmit.h"
#include "../../common/packet/content/fail_signal.h"
#include "../../common/packet/content/announcement_quit.h"

//////// Configurations ////////////////////////////////

#define TrafficTask_TIMEOUT 5 // milliseconds

////////////////////////////////////////////////////////

//////// Private Variables /////////////////////////////

// Which address or topic ID currently transmitting data?
m2tp_channel TrafficTask_currentTurn = 0;

// Caches 8 bytes of addressRegistry or topicRegistry,
// use this to prevent excessive bit shifting which
// hurts performance so much for AVR and ARM CPUs
unsigned long long TrafficTask_registryCache = 0ULL;

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void TrafficTask_nextTurn()
{
  TrafficTask_registryCache = TrafficTask_registryCache >> 1;
  TrafficTask_currentTurn++;
}

void TrafficTask_sendTurnSignal()
{
  // Abort if send listener is not ready
  if (m2tp_driver_sendListener == NULL)
    return;

  // Create packet content
  packet_content_ChannelTurnSignal content;
  content.channel = TrafficTask_currentTurn;

  // Serialize that packet content
  m2tp_byte serializedContent[CHANNEL_TURN_SIGNAL_SIZE];
  packet_content_ChannelTurnSignal_serialize(&content, serializedContent, NULL);

  // Send the channel turn signal
  m2tp_driver_sendListener(
      M2TP_COMMAND_CHANNEL_TURN_SIGNAL,
      CHANNEL_TURN_SIGNAL_SIZE,
      serializedContent);
}

// Topic ID 192 - 255
void TrafficTask_broadcastSecondBatch()
{
  while (TrafficTask_currentTurn >= 192)
  {
    if (NetworkState_isAssigned(TrafficTask_currentTurn))
    {
      TrafficTask_sendTurnSignal();
      TaskRouter_startTimeout(TrafficTask_TIMEOUT);
      return;
    }
    TrafficTask_nextTurn();
  }

  // Reset cache
  TrafficTask_registryCache = 0ULL;

  // Stop current task then go on the next task
  TaskRouter_nextTask();
}

// Topic ID 128 - 191
void TrafficTask_broadcastFirstBatch()
{
  while (TrafficTask_currentTurn < 192)
  {
    if (NetworkState_isAssigned(TrafficTask_currentTurn))
    {
      TrafficTask_sendTurnSignal();
      TaskRouter_startTimeout(TrafficTask_TIMEOUT);
      return;
    }
    TrafficTask_nextTurn();
  }

  // Put first 8 bytes of topicRegistry to cache
  TrafficTask_registryCache = NetworkState_topicRegistry[0];

  // Then pass to the next batch
  TrafficTask_broadcastSecondBatch();
}

// Device address 64 - 127
void TrafficTask_peerSecondBatch()
{
  while (TrafficTask_currentTurn < 128)
  {
    if (NetworkState_isAssigned(TrafficTask_currentTurn))
    {
      TrafficTask_sendTurnSignal();
      TaskRouter_startTimeout(TrafficTask_TIMEOUT);
      return;
    }
    TrafficTask_nextTurn();
  }

  // Put last 8 bytes of topicRegistry to cache
  TrafficTask_registryCache = NetworkState_topicRegistry[1];

  // Then pass to the next batch
  TrafficTask_broadcastFirstBatch();
}

// Device address 1 - 63
void TrafficTask_peerFirstBatch()
{
  while (TrafficTask_currentTurn < 64)
  {
    if (NetworkState_isAssigned(TrafficTask_currentTurn))
    {
      TrafficTask_sendTurnSignal();
      TaskRouter_startTimeout(TrafficTask_TIMEOUT);
      return;
    }
    TrafficTask_nextTurn();
  }

  // Put first 8 bytes of addessRegistry to cache
  TrafficTask_registryCache = NetworkState_addressRegistry[0];

  // Then pass to the next batch
  TrafficTask_peerSecondBatch();
}

void TrafficTask_continueBatch()
{
  if (TrafficTask_currentTurn < 64)
  {
    TrafficTask_nextTurn();
    TrafficTask_peerFirstBatch();
  }

  else if (TrafficTask_currentTurn < 128)
  {
    TrafficTask_nextTurn();
    TrafficTask_peerSecondBatch();
  }

  else if (TrafficTask_currentTurn < 192)
  {
    TrafficTask_nextTurn();
    TrafficTask_broadcastFirstBatch();
  }

  else
  {
    TrafficTask_nextTurn();
    TrafficTask_broadcastSecondBatch();
  }
}

void TrafficTask_receiveInterrupt(Packet *packet)
{
  // Stop the timeout
  TaskRouter_stopTimeout();

  switch (packet->command)
  {

  case M2TP_COMMAND_TRANSMIT:
  {
    // Parse the received packet content
    m2tp_byte receivedData[packet->contentSize - 2];
    packet_content_Transmit receivedContent = {.data = receivedData};
    packet_content_Transmit_parse(packet->content, packet->contentSize, &receivedContent);

    // Is this broadcasted transmit?
    if (receivedContent.target & 0b10000000U)
    {
      // Is it for me?
      if (DeviceState_isTopicAssigned(receivedContent.target))
      {
        // Forward to app
        if (m2tp_receivedListener != NULL)
          m2tp_receivedListener(receivedContent.source, receivedContent.dataSize, receivedData);
      }

      // Send success signal
      if (m2tp_driver_sendListener != NULL)
        m2tp_driver_sendListener(M2TP_COMMAND_SUCCESS_SIGNAL, 0, NULL);

      TrafficTask_continueBatch();
    }

    // ...or peer-to-peer transmit?
    else
    {
      // Is it for me?
      if (receivedContent.target == 0)
      {
        // Forward to app
        if (m2tp_receivedListener != NULL)
          m2tp_receivedListener(receivedContent.source, receivedContent.dataSize, receivedData);

        // Send success signal
        if (m2tp_driver_sendListener != NULL)
          m2tp_driver_sendListener(M2TP_COMMAND_SUCCESS_SIGNAL, 0, NULL);

        TrafficTask_continueBatch();
      }

      // ...or for anyelse?
      else
      {
        // Start timeout then suspend task
        TaskRouter_startTimeout(TrafficTask_TIMEOUT);
      }
    }
  }
  break;

  case M2TP_COMMAND_FAIL_SIGNAL:
  case M2TP_COMMAND_SUCCESS_SIGNAL:
  {
    TrafficTask_continueBatch();
  }
  break;

  default:
  {
    // Force sender to end transmission
    if (m2tp_driver_sendListener != NULL)
      m2tp_driver_sendListener(M2TP_COMMAND_END_TRANSMISSION, 0, NULL);

    TrafficTask_continueBatch();
  }
  break;

  } // switch (packet->command)
}

void TrafficTask_timeoutInterrupt()
{
  // Is currently peer-to-peer turn?
  if (TrafficTask_currentTurn < 128)
  {
    // Mark current address as unassigned
    NetworkState_unassign(TrafficTask_currentTurn);

    // Create packet content for announcement
    packet_content_AnnouncementQuit content;
    content.address = TrafficTask_currentTurn;

    // Serialize that packet content
    m2tp_byte serializedContent[ANNOUNCEMENT_QUIT_SIZE];
    packet_content_AnnouncementQuit_serialize(&content, serializedContent, NULL);

    // Send the announcement
    if (m2tp_driver_sendListener != NULL)
      m2tp_driver_sendListener(M2TP_COMMAND_ANNOUNCEMENT_QUIT, ANNOUNCEMENT_QUIT_SIZE, serializedContent);

    // Notify to App about member quit
    if (m2tp_onAnotherMemberQuitListener != NULL)
      m2tp_onAnotherMemberQuitListener(TrafficTask_currentTurn);
  }

  TrafficTask_continueBatch();
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void TrafficTask_start()
{
  // Assign functions to router
  TaskRouter_receiveInterrupt = &TrafficTask_receiveInterrupt;
  TaskRouter_timeoutInterrupt = &TrafficTask_timeoutInterrupt;

  // Put last 8 bytes of addessRegistry to cache
  TrafficTask_registryCache = NetworkState_addressRegistry[1];

  // Go to the first turn (member with address 1)
  TrafficTask_nextTurn();

  // Run the first batch, then it will let the next batch run after finished
  TrafficTask_peerFirstBatch();
}

void TrafficTask_stop()
{
  // Stop timeout
  TaskRouter_stopTimeout();

  // Detach functions
  TaskRouter_receiveInterrupt = NULL;
  TaskRouter_timeoutInterrupt = NULL;

  // Reset task
  TrafficTask_currentTurn = 0;
  TrafficTask_registryCache = 0ULL;
}

////////////////////////////////////////////////////////
