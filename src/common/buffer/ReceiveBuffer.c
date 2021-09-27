/**
 * @file ReceiveBuffer.c
 * @brief Implementation of ReceiveBuffer.h
**/

#include "ReceiveBuffer.h"

#include "m2tp/errors.h"
#include "../TaskRouter.h"

//////// Variables /////////////////////////////////////

m2tp_byte ReceiveBuffer_command = 0;
m2tp_byte ReceiveBuffer_buffer[255];
m2tp_byte ReceiveBuffer_size = 0;
m2tp_byte ReceiveBuffer_position = 0;
m2tp_error ReceiveBuffer_errorCode = 0;

////////////////////////////////////////////////////////

//////// Syntactic Sugars //////////////////////////////

#define isInitialized (ReceiveBuffer_command >= 0b10100000)
#define isExceedSize (ReceiveBuffer_position >= ReceiveBuffer_size)
#define isIncomplete (ReceiveBuffer_position < ReceiveBuffer_size)

////////////////////////////////////////////////////////

//////// Private Functions /////////////////////////////

void ReceiveBuffer_reset()
{
  ReceiveBuffer_command = 0;
  ReceiveBuffer_size = 0;
  ReceiveBuffer_position = 0;
  ReceiveBuffer_errorCode = 0;
}

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

bool ReceiveBuffer_start(
    m2tp_byte packetCommand,
    m2tp_byte packetContentSize)
{
  // Start can be "restart" if not stopped yet
  ReceiveBuffer_reset();

  // Making sure command is valid
  if (packetCommand < 0b10100000 || packetCommand > 0b10101111)
    return false;

  ReceiveBuffer_command = packetCommand;
  ReceiveBuffer_size = packetContentSize;
  return true;
}

m2tp_byte ReceiveBuffer_write(m2tp_byte value)
{
  // Abort on error
  if (ReceiveBuffer_errorCode > 0)
    return 0;

  // Abort if not started yet
  if (!isInitialized)
  {
    ReceiveBuffer_errorCode = M2TP_ERROR_UNINITIALIZED;
    return 0;
  }

  // Reset then abort if value exceed size
  if (isExceedSize)
  {
    ReceiveBuffer_reset();
    ReceiveBuffer_errorCode = M2TP_ERROR_PACKET_SIZE_MISMATCH;
    return 0;
  }

  ReceiveBuffer_buffer[ReceiveBuffer_position] = value;
  ReceiveBuffer_position++;
  return ReceiveBuffer_size - ReceiveBuffer_position;
}

m2tp_error ReceiveBuffer_finish()
{
  // Reset then abort on known error
  if (ReceiveBuffer_errorCode)
  {
    m2tp_error errorCode = ReceiveBuffer_errorCode;
    ReceiveBuffer_reset();
    return errorCode;
  }

  // Abort if not started yet
  if (!isInitialized)
    return M2TP_ERROR_UNINITIALIZED;

  // Reset then abort if incomplete write
  if (isIncomplete)
  {
    ReceiveBuffer_reset();
    return M2TP_ERROR_PACKET_SIZE_MISMATCH;
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
  return 0;
}

void ReceiveBuffer_abort()
{
  ReceiveBuffer_reset();
}

////////////////////////////////////////////////////////
