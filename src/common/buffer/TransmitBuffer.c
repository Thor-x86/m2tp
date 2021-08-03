/**
 * @file TransmitBuffer.c
 * @brief Implementation of TransmitBuffer.h
**/

#include "TransmitBuffer.h"

#include "m2tp/errors.h"
#include "m2tp/commands.h"

#include "../TaskRouter.h"
#include "../DeviceState.h"

//////// Variables /////////////////////////////////////

m2tp_byte TransmitBuffer_buffer[255];
m2tp_byte TransmitBuffer_errorCode = 0;

// TransmitBuffer holds the real pending packet
// whereas MainTask or TransmitTask only holds its pointer
Packet TransmitBuffer_packet = {0, 0, TransmitBuffer_buffer};

////////////////////////////////////////////////////////

//////// Syntactic Sugars //////////////////////////////

#define source TransmitBuffer_packet.content[0]
#define target TransmitBuffer_packet.content[1]
#define position TransmitBuffer_packet.contentSize
#define isInitialized (TransmitBuffer_packet.command == M2TP_COMMAND_TRANSMIT)
#define isSizeTooBig (position == 255)
#define isTopicID(channel) (channel & 0b10000000)

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void TransmitBuffer_reset()
{
  TransmitBuffer_packet.command = 0;
  position = 0;
  TransmitBuffer_errorCode = 0;
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void TransmitBuffer_startPeer(m2tp_channel targetAddress)
{
  while (TaskRouter_hasPendingData())
  {
    // Block current thread until there is no pending data.
    // We need to do this to make sure there is no missing packets.
  }

  // TransmitBuffer only do cleanup at beginning or when error caught
  if (isInitialized)
    TransmitBuffer_reset();

  // Making sure target address is not Topic ID
  if (isTopicID(targetAddress))
  {
    TransmitBuffer_errorCode = M2TP_ERROR_ADDRESS_NOT_EXIST;
    return;
  }

  // Prepare the content header
  TransmitBuffer_packet.command = M2TP_COMMAND_TRANSMIT;
  source = DeviceState_assignedAddress;
  target = targetAddress;
  position = 2;
}

void TransmitBuffer_startBroadcast(m2tp_channel topicID)
{
  while (TaskRouter_hasPendingData())
  {
    // Block current thread until there is no pending data.
    // We need to do this to make sure there is no missing packets.
  }

  // TransmitBuffer only do cleanup at beginning or when error caught
  if (isInitialized)
    TransmitBuffer_reset();

  // Making sure topic ID is not device address
  if (!isTopicID(topicID))
  {
    TransmitBuffer_errorCode = M2TP_ERROR_TOPIC_NOT_EXIST;
    return;
  }

  // Prepare the content header
  TransmitBuffer_packet.command = M2TP_COMMAND_TRANSMIT;
  source = DeviceState_assignedAddress;
  target = topicID;
  position = 2;
}

void TransmitBuffer_write(m2tp_byte value)
{
  // Don't write anything if it's not safe to write
  if (!isInitialized || TaskRouter_hasPendingData() || TransmitBuffer_errorCode != 0)
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
    return;

  // Found error? reset the buffer to prevent further problem
  if (TransmitBuffer_errorCode != 0)
    TransmitBuffer_reset();

  // Otherwise, submit the packet to TaskRouter
  else
    TaskRouter_sendPacket(&TransmitBuffer_packet);
}

void TransmitBuffer_finishAsync(m2tp_OnSuccessCallback successCallback, m2tp_OnErrorCallback errorCallback)
{
  // Abort if not started yet
  if (!isInitialized)
    return;

  // Found error?
  if (TransmitBuffer_errorCode != 0)
  {
    // Report it via errorCallback
    if (errorCallback != NULL)
      errorCallback(TransmitBuffer_errorCode);

    // Then reset the buffer
    TransmitBuffer_reset();
  }

  // Otherwise, send it via TaskRouter
  // then it will call the callback itself
  else
  {
    TaskRouter_sendPacketAsync(
        &TransmitBuffer_packet,
        successCallback,
        errorCallback);
  }
}

void TransmitBuffer_abort()
{
  TransmitBuffer_reset();
}

////////////////////////////////////////////////////////
