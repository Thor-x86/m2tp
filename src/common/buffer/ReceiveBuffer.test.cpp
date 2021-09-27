/**
 * @file ReceiveBuffer.test.cpp
 * @brief Unit test for ReceiveBuffer.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "ReceiveBuffer.h"
#include "m2tp/commands.h"
#include "m2tp/errors.h"
#include "../TaskRouter.h"
#include "../packet/Packet.h"
#include "../packet/content/announcement_join.h"
}

// Global variables for ReceiveBuffer test suite
namespace ReceiveBufferVars
{
  bool isReceiveInterrupt;
}

// Use the global variables only for this file
using namespace ReceiveBufferVars;

TEST(ReceiveBuffer, DefaultValue)
{
  EXPECT_EQ(ReceiveBuffer_command, 0);
  EXPECT_EQ(sizeof(ReceiveBuffer_buffer), 255);
  EXPECT_EQ(ReceiveBuffer_size, 0);
  EXPECT_EQ(ReceiveBuffer_position, 0);
}

TEST(ReceiveBuffer, NormalWrite)
{
  // Initialize required global variables
  isReceiveInterrupt = false;

  // Prepare test function for receive interrupt
  TaskRouter_receiveInterrupt = [](Packet *packet)
  {
    isReceiveInterrupt = true;
    ASSERT_NE(packet, nullptr);
    EXPECT_EQ(packet->command, M2TP_COMMAND_ANNOUNCEMENT_JOIN);
    EXPECT_EQ(packet->contentSize, 6);
    EXPECT_EQ(packet->content[0], 0x17);
    EXPECT_EQ(packet->content[1], 'L');
    EXPECT_EQ(packet->content[2], 'o');
    EXPECT_EQ(packet->content[3], 'r');
    EXPECT_EQ(packet->content[4], 'e');
    EXPECT_EQ(packet->content[5], 'm');
  };

  // Create an example of packet content
  packet_content_AnnouncementJoin content;
  content.address = 0x17;
  content.deviceClass = "Lorem";

  // Serialize the packet content
  m2tp_byte contentSize = 0;
  m2tp_byte serializedContent[255];
  packet_content_AnnouncementJoin_serialize(&content, serializedContent, &contentSize);

  // Simulate start of receive event
  bool startSuccess = ReceiveBuffer_start(M2TP_COMMAND_ANNOUNCEMENT_JOIN, contentSize);
  ASSERT_TRUE(startSuccess) << "Refused to start";

  // Simulate receive stream
  for (m2tp_byte i = 0; i < contentSize; i++)
  {
    m2tp_byte remaining = ReceiveBuffer_write(serializedContent[i]);
    ASSERT_EQ(ReceiveBuffer_errorCode, 0) << "Caught error while writing";
    if (i > contentSize - 1)
      ASSERT_NE(remaining, 0) << "Uncaught error while writing";
    EXPECT_EQ(remaining, contentSize - i - 1);
  }

  // Simulate finish of receive event
  m2tp_error caughtError = ReceiveBuffer_finish();
  ASSERT_EQ(caughtError, ReceiveBuffer_errorCode) << "Uncaught error after finished";
  ASSERT_EQ(caughtError, 0) << "Caught error after finished";

  // Check if receive interrupt triggered
  EXPECT_TRUE(isReceiveInterrupt);

  // Callback reset
  TaskRouter_receiveInterrupt = nullptr;
}

TEST(ReceiveBuffer, ExcessiveWrite)
{
  // Initialize required global variables
  isReceiveInterrupt = false;

  // Prepare test function for receive interrupt
  TaskRouter_receiveInterrupt = [](Packet *packet)
  {
    isReceiveInterrupt = true;
  };

  // Create an example of packet content
  packet_content_AnnouncementJoin content;
  content.address = 0x17;
  content.deviceClass = "Lorem";

  // Serialize the packet content
  m2tp_byte contentSize = 0;
  m2tp_byte serializedContent[255];
  packet_content_AnnouncementJoin_serialize(&content, serializedContent, &contentSize);

  // Simulate start of receive event
  bool startSuccess = ReceiveBuffer_start(M2TP_COMMAND_ANNOUNCEMENT_JOIN, contentSize);
  ASSERT_TRUE(startSuccess) << "Refused to start";

  // Simulate receive stream
  for (m2tp_byte i = 0; i < contentSize; i++)
  {
    m2tp_byte remaining = ReceiveBuffer_write(serializedContent[i]);
    ASSERT_EQ(ReceiveBuffer_errorCode, 0) << "Caught error while writing";
    if (i > contentSize - 1)
      ASSERT_NE(remaining, 0) << "Uncaught error while writing";
    EXPECT_EQ(remaining, contentSize - i - 1);
  }

  // Simulate excessive stream
  m2tp_byte remaining = ReceiveBuffer_write(0x0);
  EXPECT_EQ(remaining, 0) << "Overflow detected";

  // Simulate finish of receive event
  m2tp_error actualError = ReceiveBuffer_errorCode;
  m2tp_error caughtError = ReceiveBuffer_finish();
  ASSERT_EQ(caughtError, actualError) << "Error code not returned";
  ASSERT_EQ(caughtError, M2TP_ERROR_PACKET_SIZE_MISMATCH) << "Incorrect error code";

  // Receive interrupt shouldn't be triggered
  EXPECT_FALSE(isReceiveInterrupt);

  // Callback reset
  TaskRouter_receiveInterrupt = nullptr;
}

TEST(ReceiveBuffer, IncompleteWrite)
{
  // Initialize required global variables
  isReceiveInterrupt = false;

  // Prepare test function for receive interrupt
  TaskRouter_receiveInterrupt = [](Packet *packet)
  {
    isReceiveInterrupt = true;
  };

  // Create an example of packet content
  packet_content_AnnouncementJoin content;
  content.address = 0x17;
  content.deviceClass = "Lorem";

  // Serialize the packet content
  m2tp_byte contentSize = 0;
  m2tp_byte serializedContent[255];
  packet_content_AnnouncementJoin_serialize(&content, serializedContent, &contentSize);

  // Simulate start of receive event
  bool startSuccess = ReceiveBuffer_start(M2TP_COMMAND_ANNOUNCEMENT_JOIN, contentSize);
  ASSERT_TRUE(startSuccess) << "Refused to start";

  // Simulate incomplete receive stream
  for (m2tp_byte i = 0; i < contentSize - 1; i++)
  {
    m2tp_byte remaining = ReceiveBuffer_write(serializedContent[i]);
    ASSERT_EQ(ReceiveBuffer_errorCode, 0) << "Caught error while writing";
    ASSERT_NE(remaining, 0) << "Uncaught error while writing";
    EXPECT_EQ(remaining, contentSize - i - 1);
  }

  // Simulate finish of receive event
  m2tp_error caughtError = ReceiveBuffer_finish();
  ASSERT_NE(caughtError, 0) << "Error code not returned";
  ASSERT_EQ(caughtError, M2TP_ERROR_PACKET_SIZE_MISMATCH) << "Incorrect error code";

  // Receive interrupt shouldn't be triggered
  EXPECT_FALSE(isReceiveInterrupt);

  // Callback reset
  TaskRouter_receiveInterrupt = nullptr;
}

TEST(ReceiveBuffer, UninitializedWrite)
{
  // Initialize required global variables
  isReceiveInterrupt = false;

  // Prepare test function for receive interrupt
  TaskRouter_receiveInterrupt = [](Packet *packet)
  {
    isReceiveInterrupt = true;
  };

  // Simulate unitialized write with NULL
  ReceiveBuffer_write((m2tp_byte)NULL);
  EXPECT_EQ(ReceiveBuffer_position, 0);

  // Simulate another unitialized write
  ReceiveBuffer_write(0x7f);
  EXPECT_EQ(ReceiveBuffer_position, 0);

  // Simulate finish with unitialized condition
  m2tp_error actualError = ReceiveBuffer_errorCode;
  m2tp_error caughtError = ReceiveBuffer_finish();
  ASSERT_EQ(caughtError, actualError) << "Error code not returned";
  ASSERT_EQ(caughtError, M2TP_ERROR_UNINITIALIZED) << "Incorrect error code";
  EXPECT_FALSE(isReceiveInterrupt) << "Interrupt should not be triggered";

  // Callback reset
  TaskRouter_receiveInterrupt = nullptr;
}
