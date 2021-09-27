/**
 * @file TransmitBuffer.h
 * @brief Data stream memory management for: App => M2TP
**/

#ifndef IS_TRANSMIT_BUFFER_DEFINED
#define IS_TRANSMIT_BUFFER_DEFINED

#include "m2tp/typedef.h"
#include "m2tp/interface/app.h"
#include "../packet/Packet.h"

/**
 * @brief Prepare data buffer for peer-to-peer transmit
 * @param targetAddress Audience device's address
 * @return Error code, 0 means success
 */
extern m2tp_error TransmitBuffer_startPeer(m2tp_channel targetAddress);

/**
 * @brief Prepare data buffer for broadcast transmit
 * @param topicID Assigned topic ID
 * @return Error code, 0 means success
 */
extern m2tp_error TransmitBuffer_startBroadcast(m2tp_channel topicID);

/**
 * @brief Stream byte to buffer
 * @param value a Single byte of data
 * @return Error code, 0 means success
 */
extern m2tp_error TransmitBuffer_write(m2tp_byte value);

/**
 * @brief Send buffer to TransmitTask, then clean the buffer
 * @return Error code, 0 means success
 */
extern m2tp_error TransmitBuffer_finish();

/**
 * @brief Like TransmitBuffer_finish, but this allows app to prevent thread blocking
 * @param successCallback Pointer to a function, will be called if data transmitted
 * @param errorCallback Pointer to a function, will be called if something goes wrong
 */
extern void TransmitBuffer_finishAsync(m2tp_OnSuccessCallback successCallback, m2tp_OnErrorCallback errorCallback);

/**
 * @brief Just clean the buffer without sending
 */
extern void TransmitBuffer_abort();

// Expose private variables to unit test framework
#ifdef GOOGLETEST_INCLUDE_GTEST_GTEST_H_
extern m2tp_byte TransmitBuffer_buffer[253];
extern m2tp_byte TransmitBuffer_errorCode;
extern Packet TransmitBuffer_packet;
#endif

#endif // #ifndef IS_TRANSMIT_BUFFER_DEFINED
