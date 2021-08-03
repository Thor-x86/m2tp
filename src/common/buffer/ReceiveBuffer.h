/**
 * @file ReceiveBuffer.h
 * @brief Packet stream memory management for: Driver => M2TP
**/

#ifndef IS_RECEIVE_BUFFER_DEFINED
#define IS_RECEIVE_BUFFER_DEFINED

#include "m2tp/typedef.h"

/**
 * @brief Prepare packet buffer
 * @param packetCommand See "m2tp/commands.h"
 * @param packetContentSize Size of packet content, maximum 255 bytes
 */
extern void ReceiveBuffer_start(m2tp_byte packetCommand, m2tp_byte packetContentSize);

/**
 * @brief Stream byte to buffer
 * @param value a Single byte of packet content
 */
extern void ReceiveBuffer_write(m2tp_byte value);

/**
 * @brief Send the buffer to TaskRouter, then clear the buffer
 */
extern void ReceiveBuffer_finish();

/**
 * @brief Just clean the buffer, will be called on error
 */
extern void ReceiveBuffer_abort();

// Expose private variables to unit test framework
#ifdef GOOGLETEST_INCLUDE_GTEST_GTEST_H_
extern m2tp_byte ReceiveBuffer_command;
extern m2tp_byte ReceiveBuffer_buffer[255];
extern m2tp_byte ReceiveBuffer_size;
extern m2tp_byte ReceiveBuffer_position;
#endif

#endif // #ifndef IS_RECEIVE_BUFFER_DEFINED
