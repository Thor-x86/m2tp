/**
 * @file CanHook.c
 * @brief Implementation of CanHook.h
**/

#include "CanHook.h"
#include "dependencies.h"

unsigned int CanHook_ID = 0;

m2tp_byte CanHook_receive(
    m2tp_bytes input,
    m2tp_byte inputSize,
    m2tp_bytes output)
{
  // Parse input
  struct can_frame canFrame;
  memcpy(&canFrame, input, inputSize);

  // Ignore if CAN ID not match
  if (canFrame.can_id != CanHook_ID)
    return 0;

  // Get the frame data and put it as output
  memcpy(output, &(canFrame.data), canFrame.len);

  // Get the output size
  return canFrame.len;
}

m2tp_byte CanHook_transmit(
    m2tp_bytes input,
    m2tp_byte inputSize,
    m2tp_bytes output)
{
  // Create CAN Frame
  struct can_frame canFrame;
  memset(&canFrame, 0, sizeof(canFrame));

  // Insert the frame data size and CAN ID
  canFrame.len = inputSize;
  canFrame.can_id = CanHook_ID;

  // Insert serialized packet as frame data
  memcpy(&(canFrame.data), input, inputSize);

  // Put frame as output, also its size
  memcpy(output, &canFrame, sizeof(canFrame));
  return sizeof(canFrame);
}
