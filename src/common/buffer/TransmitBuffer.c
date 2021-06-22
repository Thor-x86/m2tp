/**
 * @file TransmitBuffer.c
 * @brief Implementation of TransmitBuffer.h
**/

#include "TransmitBuffer.h"

#include "m2tp-common/errors.h"
#include "m2tp-common/commands.h"
#include "../task/TransmitTask.h"

//////// Compile-time Constants ////////////////////////

#define POSITION_UNINITIALIZED 255
#define POSITION_OVERLOAD 254

////////////////////////////////////////////////////////

//////// Variables /////////////////////////////////////

m2tp_channel TransmitBuffer_destination = NULL;
m2tp_byte TransmitBuffer_buffer[253];
m2tp_byte TransmitBuffer_errorCode = NULL;

// TransmitBuffer holds the real pending packet
// whereas TransmitTask only holds its pointer
Packet TransmitBuffer_packet = {
    M2TP_COMMAND_TRANSMIT,
    POSITION_UNINITIALIZED,
    TransmitBuffer_buffer};

////////////////////////////////////////////////////////

//////// Syntactic Sugars //////////////////////////////

#define position TransmitBuffer_packet.contentSize
#define isInitialized (position != POSITION_UNINITIALIZED)
#define isSizeTooBig (position == POSITION_OVERLOAD)

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void TransmitBuffer_reset()
{
  TransmitBuffer_destination = NULL;
  position = POSITION_UNINITIALIZED;
  TransmitBuffer_errorCode = NULL;
}

bool TransmitBuffer_isTopicID(m2tp_channel channel)
{
  // TopicID has 1 at the leftmost,
  // device address not.
  return (channel & 0b10000000);
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void TransmitBuffer_startPeer(m2tp_channel targetAddress)
{
  while (TransmitTask_hasPendingPacket())
  {
    // Block current thread until TransmitTask ready
  }

  // Start can be "restart" if app didn't call finish yet
  if (isInitialized)
    TransmitBuffer_reset();

  // Making sure target address is not Topic ID
  if (TransmitBuffer_isTopicID(targetAddress))
  {
    TransmitBuffer_errorCode = M2TP_ERROR_ADDRESS_NOT_EXIST;
    return;
  }

  position = 0;
  TransmitBuffer_destination = targetAddress;
}

void TransmitBuffer_startBroadcast(m2tp_channel topicID)
{
  while (TransmitTask_hasPendingPacket())
  {
    // Block current thread until TransmitTask ready
  }

  // Start can be "restart" if app didn't call finish yet
  if (isInitialized)
    TransmitBuffer_reset();

  // Making sure topic ID is not device address
  if (!TransmitBuffer_isTopicID(topicID))
  {
    TransmitBuffer_errorCode = M2TP_ERROR_TOPIC_NOT_EXIST;
    return;
  }

  position = 0;
  TransmitBuffer_destination = topicID;
}

void TransmitBuffer_write(m2tp_byte value)
{
  // Abort if not started yet,
  // or currently waiting for TransmitTask
  if (!isInitialized || TransmitTask_hasPendingPacket())
    return;

  // Too much bytes? report then abort
  if (isSizeTooBig)
  {
    TransmitBuffer_errorCode = M2TP_ERROR_DATA_SIZE_TOO_BIG;
    return;
  }

  TransmitBuffer_buffer[position] = value;
  position++;
}

void TransmitBuffer_finish()
{
  // Abort if not started yet
  if (!isInitialized)
  {
    TransmitBuffer_reset();
    return;
  }

  // Send to TransmitTask, if no error
  if (TransmitBuffer_errorCode == NULL)
    TransmitTask_send(&TransmitBuffer_packet);

  // Cleanup
  TransmitBuffer_reset();
}

void TransmitBuffer_finishAsync(m2tp_OnSuccessCallback successCallback, m2tp_OnErrorCallback errorCallback)
{
  // Abort if not started yet
  if (!isInitialized)
  {
    TransmitBuffer_reset();
    return;
  }

  // Found error? report it via errorCallback
  if (TransmitBuffer_errorCode != NULL)
  {
    if (errorCallback != NULL)
      errorCallback(TransmitBuffer_errorCode);
  }

  // Otherwise, send it to TransmitTask
  // then it will call the callback itself
  else
  {
    TransmitTask_send(&TransmitBuffer_packet);
  }

  // Cleanup
  TransmitBuffer_reset();
}

void TransmitBuffer_abort()
{
  TransmitBuffer_reset();
}

////////////////////////////////////////////////////////
