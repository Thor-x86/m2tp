/**
 * @file ReceiveBuffer.c
 * @brief Implementation of ReceiveBuffer.h
**/

#include "ReceiveBuffer.h"

#include "m2tp-common/errors.h"
#include "../TaskRouter.h"

//////// Variables /////////////////////////////////////

m2tp_byte ReceiveBuffer_command = NULL;
m2tp_byte ReceiveBuffer_buffer[255];
m2tp_byte ReceiveBuffer_size = 0;
m2tp_byte ReceiveBuffer_position = 0;

////////////////////////////////////////////////////////

//////// Syntactic Sugars //////////////////////////////

#define isInitialized (ReceiveBuffer_command >= 0b10100000)
#define isExceedSize (ReceiveBuffer_position >= ReceiveBuffer_size)
#define isIncomplete (ReceiveBuffer_position < ReceiveBuffer_size)

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void ReceiveBuffer_reset()
{
  ReceiveBuffer_command = NULL;
  ReceiveBuffer_size = 0;
  ReceiveBuffer_position = 0;
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void ReceiveBuffer_start(
    m2tp_byte packetCommand,
    m2tp_byte packetContentSize)
{
  // Start can be "restart" if not stopped yet
  if (isInitialized)
    ReceiveBuffer_reset();

  // When TaskRouter is currently switching task or not ready
  if (TaskRouter_receiveInterrupt == NULL)
    return;

  ReceiveBuffer_command = packetCommand;
  ReceiveBuffer_size = packetContentSize;
}

void ReceiveBuffer_write(m2tp_byte value)
{
  // Abort if not started yet
  if (!isInitialized)
    return;

  // Reset then abort if value exceed size
  if (isExceedSize)
  {
    ReceiveBuffer_reset();
    return;
  }

  ReceiveBuffer_buffer[ReceiveBuffer_position] = value;
  ReceiveBuffer_position++;
}

void ReceiveBuffer_finish()
{
  // Abort if not started yet
  if (!isInitialized)
    return;

  // Reset then abort if incomplete write
  if (isIncomplete)
  {
    ReceiveBuffer_reset();
    return;
  }

  // Send buffer to TaskRouter for execution
  if (TaskRouter_receiveInterrupt != NULL)
  {
    Packet packet;
    packet.command = ReceiveBuffer_command;
    packet.contentSize = ReceiveBuffer_size;
    packet.content = ReceiveBuffer_buffer;
    TaskRouter_receiveInterrupt(&packet);
  }

  // Cleanup
  ReceiveBuffer_reset();
}

void ReceiveBuffer_abort()
{
  ReceiveBuffer_reset();
}

////////////////////////////////////////////////////////
