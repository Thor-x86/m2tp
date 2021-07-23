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
#include "../DeviceState.h"
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
  EXPECT_EQ(sizeof(TransmitBuffer_buffer), 253);
  EXPECT_EQ(TransmitBuffer_errorCode, 0);
  EXPECT_EQ(TransmitBuffer_packet.command, 0);
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 0);
  EXPECT_EQ(TransmitBuffer_packet.content, (m2tp_bytes)&TransmitBuffer_buffer);
}

TEST(TransmitBuffer, NormalPeerWrite)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate device address
  DeviceState_assignedAddress = 0x7;

  // Simulate start peer-to-peer transmit
  TransmitBuffer_startPeer(0x13);

  // Check for header
  EXPECT_EQ(TransmitBuffer_packet.content[0], 0x7);
  EXPECT_EQ(TransmitBuffer_packet.content[1], 0x13);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Check for error
  ASSERT_EQ(TransmitBuffer_errorCode, 0);

  // Making sure content size is correct
  ASSERT_EQ(TransmitBuffer_packet.contentSize, dataSize + 2) << "Content size is incorrect";

  // Check for content
  for (m2tp_byte i = 0; i < dataSize; i++)
    ASSERT_EQ(TransmitBuffer_packet.content[2 + i], data[i]) << "Invalid content at #" << (int)i << " byte";

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Making sure the buffer is submitted to task
  EXPECT_EQ(MainTask_pendingTransmit, &TransmitBuffer_packet);

  // Unlike ReceiveBuffer, the TransmitBuffer must NOT be cleaned up after finish
  EXPECT_NE(TransmitBuffer_packet.command, 0) << "Command is inaccessible by TaskRouter";
  EXPECT_NE(TransmitBuffer_packet.contentSize, 0) << "Content size is inaccessible by TaskRouter";

  // Test Cleanup
  MainTask_stop();
  DeviceState_assignedAddress = 0;
}

TEST(TransmitBuffer, AsyncPeerWrite)
{
  // Prepare global variables
  isErrorCalled = false;

  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate device address
  DeviceState_assignedAddress = 0x7;

  // Simulate start peer-to-peer transmit
  TransmitBuffer_startPeer(0x13);

  // Check for header
  EXPECT_EQ(TransmitBuffer_packet.content[0], 0x7);
  EXPECT_EQ(TransmitBuffer_packet.content[1], 0x13);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Check for error
  ASSERT_EQ(TransmitBuffer_errorCode, 0);

  // Making sure content size is correct
  ASSERT_EQ(TransmitBuffer_packet.contentSize, dataSize + 2) << "Content size is incorrect";

  // Check for content
  for (m2tp_byte i = 0; i < dataSize; i++)
    ASSERT_EQ(TransmitBuffer_packet.content[2 + i], data[i]) << "Invalid content at #" << (int)i << " byte";

  // Prepare for dummy error callback
  m2tp_OnErrorCallback errorCallback = [](m2tp_byte errorCode)
  {
    isErrorCalled = true;
  };

  // Simulate asynchronous finish
  TransmitBuffer_finishAsync(NULL, errorCallback);

  // Making sure error callback called
  EXPECT_FALSE(isErrorCalled) << "Error code: " << (int)TransmitBuffer_errorCode;

  // Making sure the buffer is submitted to task
  EXPECT_EQ(MainTask_pendingTransmit, &TransmitBuffer_packet);

  // Unlike ReceiveBuffer, the TransmitBuffer must NOT be cleaned up after finish
  EXPECT_NE(TransmitBuffer_packet.command, 0) << "Command is inaccessible by TaskRouter";
  EXPECT_NE(TransmitBuffer_packet.contentSize, 0) << "Content size is inaccessible by TaskRouter";

  // Test Cleanup
  MainTask_stop();
  DeviceState_assignedAddress = 0;
}

TEST(TransmitBuffer, NormalBroadcastWrite)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate device address
  DeviceState_assignedAddress = 0x7;

  // Simulate start broadcast transmit
  TransmitBuffer_startBroadcast(0x81);

  // Check for header
  EXPECT_EQ(TransmitBuffer_packet.content[0], 0x7);
  EXPECT_EQ(TransmitBuffer_packet.content[1], 0x81);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Check for error
  ASSERT_EQ(TransmitBuffer_errorCode, 0);

  // Making sure content size is correct
  ASSERT_EQ(TransmitBuffer_packet.contentSize, dataSize + 2) << "Content size is incorrect";

  // Check for content
  for (m2tp_byte i = 0; i < dataSize; i++)
    ASSERT_EQ(TransmitBuffer_packet.content[2 + i], data[i]) << "Invalid content at #" << (int)i << " byte";

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Making sure the buffer is submitted to task
  EXPECT_EQ(MainTask_pendingTransmit, &TransmitBuffer_packet);

  // Unlike ReceiveBuffer, the TransmitBuffer must NOT be cleaned up after finish
  EXPECT_NE(TransmitBuffer_packet.command, 0) << "Command is inaccessible by TaskRouter";
  EXPECT_NE(TransmitBuffer_packet.contentSize, 0) << "Content size is inaccessible by TaskRouter";

  // Test Cleanup
  MainTask_stop();
  DeviceState_assignedAddress = 0;
}

TEST(TransmitBuffer, AsyncBroadcastWrite)
{
  // Prepare global variables
  isErrorCalled = false;

  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate device address
  DeviceState_assignedAddress = 0x7;

  // Simulate start broadcast transmit
  TransmitBuffer_startBroadcast(0x81);

  // Check for header
  EXPECT_EQ(TransmitBuffer_packet.content[0], 0x7);
  EXPECT_EQ(TransmitBuffer_packet.content[1], 0x81);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Check for error
  ASSERT_EQ(TransmitBuffer_errorCode, 0); // Making sure content size is correct
  ASSERT_EQ(TransmitBuffer_packet.contentSize, dataSize + 2) << "Content size is incorrect";

  // Check for content
  for (m2tp_byte i = 0; i < dataSize; i++)
    ASSERT_EQ(TransmitBuffer_packet.content[2 + i], data[i]) << "Invalid content at #" << (int)i << " byte";

  // Prepare for dummy error callback
  m2tp_OnErrorCallback errorCallback = [](m2tp_byte errorCode)
  {
    isErrorCalled = true;
  };

  // Simulate asynchronous finish
  TransmitBuffer_finishAsync(NULL, errorCallback);

  // Making sure error callback called
  EXPECT_FALSE(isErrorCalled) << "Error code: " << (int)TransmitBuffer_errorCode;

  // Making sure the buffer is submitted to task
  EXPECT_EQ(MainTask_pendingTransmit, &TransmitBuffer_packet);

  // Unlike ReceiveBuffer, the TransmitBuffer must NOT be cleaned up after finish
  EXPECT_NE(TransmitBuffer_packet.command, 0) << "Command is inaccessible by TaskRouter";
  EXPECT_NE(TransmitBuffer_packet.contentSize, 0) << "Content size is inaccessible by TaskRouter";

  // Test Cleanup
  MainTask_stop();
  DeviceState_assignedAddress = 0;
}

TEST(TransmitBuffer, WrongAddressTransmit)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate device address
  DeviceState_assignedAddress = 0x7;

  // Simulate start peer-to-peer transmit,
  // but confuses address with topic ID
  TransmitBuffer_startPeer(0x81);

  // Making sure TransmitBuffer aware of incorrect address
  ASSERT_EQ(TransmitBuffer_errorCode, M2TP_ERROR_ADDRESS_NOT_EXIST);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Buffer must stay empty because of error
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 0);

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Making sure it's not submitted to task yet
  EXPECT_NE(MainTask_pendingTransmit, &TransmitBuffer_packet);

  // Test Cleanup
  MainTask_stop();
  DeviceState_assignedAddress = 0;
}

TEST(TransmitBuffer, WrongTopicTransmit)
{
  // Create an example of data
  m2tp_bytes data = (m2tp_bytes) "Lorem Ipsum";
  m2tp_byte dataSize = 12;

  // Simulate device address
  DeviceState_assignedAddress = 0x7;

  // Simulate start broadcast transmit,
  // but confuses topic ID with address
  TransmitBuffer_startBroadcast(0x17);

  // Making sure TransmitBuffer aware of incorrect topic ID
  ASSERT_EQ(TransmitBuffer_errorCode, M2TP_ERROR_TOPIC_NOT_EXIST);

  // Simulate stream data from app
  for (m2tp_byte i = 0; i < dataSize; i++)
    TransmitBuffer_write(data[i]);

  // Buffer must stay empty because of error
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 0);

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Making sure it's not submitted to task yet
  EXPECT_NE(MainTask_pendingTransmit, &TransmitBuffer_packet);

  // Test Cleanup
  MainTask_stop();
  DeviceState_assignedAddress = 0;
}

TEST(TransmitBuffer, ExceedLimitTransmit)
{
  // Simulate device address
  DeviceState_assignedAddress = 0x7;

  // Simulate start peer-to-peer transmit
  TransmitBuffer_startPeer(0x13);

  // Check for the address
  ASSERT_EQ(TransmitBuffer_packet.content[1], 0x13) << "Cannot run test, address isn't assigned correctly";

  // Simulate oversized data stream
  for (int i = 0; i < 258; i++)
    TransmitBuffer_write(i);

  // Check for error
  EXPECT_EQ(TransmitBuffer_errorCode, M2TP_ERROR_DATA_SIZE_TOO_BIG);

  // Making sure content size stays at 255
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255);

  // Simulate finish of data transmit
  TransmitBuffer_finish();

  // Making sure it's not submitted to task yet
  EXPECT_NE(MainTask_pendingTransmit, &TransmitBuffer_packet);

  // Test Cleanup
  MainTask_stop();
  DeviceState_assignedAddress = 0;
}

TEST(TransmitBuffer, ExceedLimitAsyncTransmit)
{
  // Prepare global variables
  isSuccessCalled = false;
  isErrorCalled = false;

  // Simulate device address
  DeviceState_assignedAddress = 0x7;

  // Simulate start peer-to-peer transmit
  TransmitBuffer_startPeer(0x13);

  // Check for the address
  ASSERT_EQ(TransmitBuffer_packet.content[1], 0x13) << "Cannot run test, address isn't assigned correctly";

  // Simulate oversized data stream
  for (unsigned int i = 0; i < 258; i++)
    TransmitBuffer_write((m2tp_byte)i);

  // Check for error
  EXPECT_EQ(TransmitBuffer_errorCode, M2TP_ERROR_DATA_SIZE_TOO_BIG);

  // Making sure content size stays at 255
  EXPECT_EQ(TransmitBuffer_packet.contentSize, 255);

  // Prepare success callback
  m2tp_OnSuccessCallback successCallback = []()
  {
    isSuccessCalled = true;
  };

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

  // Test Cleanup
  MainTask_stop();
  DeviceState_assignedAddress = 0;
}
