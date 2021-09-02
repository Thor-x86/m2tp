/**
 * @file CanHook.c
 * @brief Implementation of CanHook.h
**/

#include "CanHook.h"
#include "dependencies.h"

// can_dlc is deprecated but its replacement (len)
// is not available on older kernel, so we need polyfill
struct CANFramePolyfill
{
  canid_t can_id; /* 32 bit CAN_ID + EFF/RTR/ERR flags */
  union
  {
    __u8 len;
    __u8 can_dlc;
  } __attribute__((packed));
  __u8 __pad;
  __u8 __res0;
  __u8 len8_dlc;
  __u8 data[CAN_MAX_DLEN] __attribute__((aligned(8)));
};

unsigned int CanHook_ID = 0;

m2tp_byte CanHook_receive(
    m2tp_bytes input,
    m2tp_byte inputSize,
    m2tp_bytes output)
{
  // Parse input
  struct CANFramePolyfill canFrame;
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
  struct CANFramePolyfill canFrame;
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
