/**
 * @file CanHook.h
 * @brief Processes CAN bus specific frame structure
**/

#include "m2tp-posix.h"

// The value came from m2tp_connectViaCAN(...)
extern unsigned int CanHook_ID;

// Serialized CAN Frame => Serialized M2TP Packet
extern m2tp_byte CanHook_receive(m2tp_bytes input, m2tp_byte inputSize, m2tp_bytes output);

// Serialized M2TP Packet => Serialized CAN Frame
extern m2tp_byte CanHook_transmit(m2tp_bytes input, m2tp_byte inputSize, m2tp_bytes output);
