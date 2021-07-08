/**
 * @file TransmitBuffer.test.cpp
 * @brief Unit test for TransmitBuffer.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "TransmitBuffer.h"
#include "m2tp-common/commands.h"
#include "m2tp-common/errors.h"
#include "../TaskRouter.h"
#include "../../member/task/MainTask.h"
}

// Global variables for TransmitBuffer test suite
namespace TransmitBufferVars
{
  bool isSuccessCalled;
  bool isErrorCalled;
}

// Use the global variables only for this file
using namespace TransmitBufferVars;

TEST(TransmitBuffer, DefaultValue)
{
  EXPECT_EQ(TransmitBuffer_destination, 0);
  EXPECT_EQ(sizeof(TransmitBuffer_buffer), 253);
  EXPECT_EQ(TransmitBuffer_errorCode, 0);
  EXPECT_EQ(TransmitBuffer_packet.command, M2TP_COMMAND_TRANSMIT);
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255);
  EXPECT_EQ(TransmitBuffer_packet.content, (m2tp_bytes)&TransmitBuffer_buffer);
}

TEST(TransmitBuffer, NormalPeerWrite)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate start peer-to-peer transmit
  TransmitBuffer_startPeer(0x13);

  // Check for the address
  EXPECT_EQ(TransmitBuffer_destination, 0x13);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Check for error
  ASSERT_EQ(TransmitBuffer_errorCode, NULL);

  // Making sure data size is correct
  ASSERT_EQ(TransmitBuffer_packet.contentSize, dataSize);

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Check if cleaned up properly
  EXPECT_EQ(TransmitBuffer_destination, NULL) << "Destination isn't cleaned";
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255) << "Position/contentSize isn't cleaned";
  EXPECT_EQ(TransmitBuffer_errorCode, NULL) << "Error code isn't cleaned";

  // Stop the dummy task
  MainTask_stop();
}

TEST(TransmitBuffer, AsyncPeerWrite)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate start peer-to-peer transmit
  TransmitBuffer_startPeer(0x13);

  // Check for the address
  EXPECT_EQ(TransmitBuffer_destination, 0x13);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Check for error
  ASSERT_EQ(TransmitBuffer_errorCode, NULL);

  // Making sure data size is correct
  ASSERT_EQ(TransmitBuffer_packet.contentSize, dataSize);

  // Simulate asynchronous finish
  TransmitBuffer_finishAsync(nullptr, nullptr);

  // FIXME: Find a way to mock TaskRouter

  // Check if cleaned up properly
  EXPECT_EQ(TransmitBuffer_destination, NULL) << "Destination isn't cleaned";
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255) << "Position/contentSize isn't cleaned";
  EXPECT_EQ(TransmitBuffer_errorCode, NULL) << "Error code isn't cleaned";

  // Stop the dummy task
  MainTask_stop();
}

TEST(TransmitBuffer, NormalBroadcastWrite)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate start broadcast transmit
  TransmitBuffer_startBroadcast(0x81);

  // Check for topicID
  EXPECT_EQ(TransmitBuffer_destination, 0x81);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Check for error
  ASSERT_EQ(TransmitBuffer_errorCode, NULL);

  // Making sure data size is correct
  ASSERT_EQ(TransmitBuffer_packet.contentSize, dataSize);

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Check if cleaned up properly
  EXPECT_EQ(TransmitBuffer_destination, NULL) << "Destination isn't cleaned";
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255) << "Position/contentSize isn't cleaned";
  EXPECT_EQ(TransmitBuffer_errorCode, NULL) << "Error code isn't cleaned";

  // Stop the dummy task
  MainTask_stop();
}

TEST(TransmitBuffer, AsyncBroadcastWrite)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate start broadcast transmit
  TransmitBuffer_startBroadcast(0x81);

  // Check for topicID
  EXPECT_EQ(TransmitBuffer_destination, 0x81);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Check for error
  ASSERT_EQ(TransmitBuffer_errorCode, NULL);

  // Making sure data size is correct
  ASSERT_EQ(TransmitBuffer_packet.contentSize, dataSize);

  // Simulate asynchronous finish
  TransmitBuffer_finishAsync(nullptr, nullptr);

  // FIXME: Find a way to mock TaskRouter

  // Check if cleaned up properly
  EXPECT_EQ(TransmitBuffer_destination, NULL) << "Destination isn't cleaned";
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255) << "Position/contentSize isn't cleaned";
  EXPECT_EQ(TransmitBuffer_errorCode, NULL) << "Error code isn't cleaned";

  // Stop the dummy task
  MainTask_stop();
}

TEST(TransmitBuffer, WrongAddressTransmit)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate start peer-to-peer transmit,
  // but confuses address with topic ID
  TransmitBuffer_startPeer(0x81);

  // Making sure TransmitBuffer aware of incorrect address
  EXPECT_NE(TransmitBuffer_destination, 0x81);
  ASSERT_EQ(TransmitBuffer_errorCode, M2TP_ERROR_ADDRESS_NOT_EXIST);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Buffer must stay empty because of error
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255);

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Check if cleaned up properly
  EXPECT_EQ(TransmitBuffer_destination, NULL) << "Destination isn't cleaned";
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255) << "Position/contentSize isn't cleaned";
  EXPECT_EQ(TransmitBuffer_errorCode, NULL) << "Error code isn't cleaned";

  // Stop the dummy task
  MainTask_stop();
}

TEST(TransmitBuffer, WrongTopicTransmit)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate start broadcast transmit,
  // but confuses topic ID with address
  TransmitBuffer_startBroadcast(0x17);

  // Making sure TransmitBuffer aware of incorrect topic ID
  EXPECT_NE(TransmitBuffer_destination, 0x17);
  ASSERT_EQ(TransmitBuffer_errorCode, M2TP_ERROR_TOPIC_NOT_EXIST);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Buffer must stay empty because of error
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255);

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Check if cleaned up properly
  EXPECT_EQ(TransmitBuffer_destination, NULL) << "Destination isn't cleaned";
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255) << "Position/contentSize isn't cleaned";
  EXPECT_EQ(TransmitBuffer_errorCode, NULL) << "Error code isn't cleaned";

  // Stop the dummy task
  MainTask_stop();
}

TEST(TransmitBuffer, ExceedLimitTransmit)
{
  // Simulate start peer-to-peer transmit
  TransmitBuffer_startPeer(0x13);

  // Check for the address
  ASSERT_EQ(TransmitBuffer_destination, 0x13) << "Cannot run test, address isn't assigned correctly";

  // Simulate oversized data stream
  for (m2tp_byte i = 0; i < 255; i++)
    TransmitBuffer_write(i);

  // Check for error
  EXPECT_EQ(TransmitBuffer_errorCode, M2TP_ERROR_DATA_SIZE_TOO_BIG);

  // Making sure data equals 254,
  // which means content is exceeding limit
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 254);

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Check if cleaned up properly
  EXPECT_EQ(TransmitBuffer_destination, NULL) << "Destination isn't cleaned";
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255) << "Position/contentSize isn't cleaned";
  EXPECT_EQ(TransmitBuffer_errorCode, NULL) << "Error code isn't cleaned";

  // Stop the dummy task
  MainTask_stop();
}

TEST(TransmitBuffer, ExceedLimitAsyncTransmit)
{
  // Prepare global variables
  isSuccessCalled = false;
  isErrorCalled = false;

  // Simulate start peer-to-peer transmit
  TransmitBuffer_startPeer(0x13);

  // Check for the address
  ASSERT_EQ(TransmitBuffer_destination, 0x13) << "Cannot run test, address isn't assigned correctly";

  // Simulate oversized data stream
  for (unsigned int i = 0; i < 256; i++)
    TransmitBuffer_write((m2tp_byte)i);

  // Check for error
  EXPECT_EQ(TransmitBuffer_errorCode, M2TP_ERROR_DATA_SIZE_TOO_BIG);

  // Making sure data equals 254,
  // which means content is exceeding limit
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 254);

  // Prepare success callback
  m2tp_OnSuccessCallback successCallback = []()
  { isSuccessCalled = true; };

  // Prepare error callback
  m2tp_OnErrorCallback errorCallback = [](m2tp_byte errorCode)
  {
    EXPECT_EQ(errorCode, M2TP_ERROR_DATA_SIZE_TOO_BIG);
    isErrorCalled = true;
  };

  // Simulate finish of data transmit, asynchronously
  TransmitBuffer_finishAsync(successCallback, errorCallback);

  // Check if only error callback being called
  EXPECT_FALSE(isSuccessCalled);
  EXPECT_TRUE(isErrorCalled);

  // Check if cleaned up properly
  EXPECT_EQ(TransmitBuffer_destination, NULL) << "Destination isn't cleaned";
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255) << "Position/contentSize isn't cleaned";
  EXPECT_EQ(TransmitBuffer_errorCode, NULL) << "Error code isn't cleaned";

  // Stop the dummy task
  MainTask_stop();
}
