/**
 * @file ReceiveBuffer.test.cpp
 * @brief Unit test for ReceiveBuffer.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "ReceiveBuffer.h"
#include "m2tp-common/commands.h"
#include "m2tp-common/errors.h"
#include "../TaskRouter.h"
#include "../packet/Packet.h"
#include "../packet/content/announcement_join.h"
}

// Global variables for ReceiveBuffer test suite
namespace ReceiveBufferVars
{
  bool isReceiveInterrupt;
  bool isErrorReported;
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
  isErrorReported = false;

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

  // Prepare test function for error report
  TaskRouter_reportReceiveFail = [](m2tp_byte errorCode)
  {
    isErrorReported = true;
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
  ReceiveBuffer_start(M2TP_COMMAND_ANNOUNCEMENT_JOIN, contentSize);

  // Simulate receive stream
  for (m2tp_byte i = 0; i < contentSize; i++)
    ReceiveBuffer_write(serializedContent[i]);

  // Simulate finish of receive event
  ReceiveBuffer_finish();

  // Check if receive interrupt triggered
  EXPECT_TRUE(isReceiveInterrupt);

  // Making sure no error detected
  EXPECT_FALSE(isErrorReported);

  // Callback reset
  TaskRouter_receiveInterrupt = nullptr;
  TaskRouter_reportReceiveFail = nullptr;
}

TEST(ReceiveBuffer, ExcessiveWrite)
{
  // Initialize required global variables
  isReceiveInterrupt = false;
  isErrorReported = false;

  // Prepare test function for receive interrupt
  TaskRouter_receiveInterrupt = [](Packet *packet)
  {
    isReceiveInterrupt = true;
  };

  // Prepare test function for error report
  TaskRouter_reportReceiveFail = [](m2tp_byte errorCode)
  {
    isErrorReported = true;
    EXPECT_EQ(errorCode, M2TP_ERROR_PACKET_SIZE_MISMATCH);
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
  ReceiveBuffer_start(M2TP_COMMAND_ANNOUNCEMENT_JOIN, contentSize);

  // Simulate receive stream
  for (m2tp_byte i = 0; i < contentSize; i++)
    ReceiveBuffer_write(serializedContent[i]);

  // Simulate excessive stream
  ReceiveBuffer_write(0x0);

  // Simulate finish of receive event
  ReceiveBuffer_finish();

  // Receive interrupt shouldn't be triggered
  EXPECT_FALSE(isReceiveInterrupt);

  // Check if error reported
  EXPECT_TRUE(isErrorReported);

  // Callback reset
  TaskRouter_receiveInterrupt = nullptr;
  TaskRouter_reportReceiveFail = nullptr;
}

TEST(ReceiveBuffer, IncompleteWrite)
{
  // Initialize required global variables
  isReceiveInterrupt = false;
  isErrorReported = false;

  // Prepare test function for receive interrupt
  TaskRouter_receiveInterrupt = [](Packet *packet)
  {
    isReceiveInterrupt = true;
  };

  // Prepare test function for error report
  TaskRouter_reportReceiveFail = [](m2tp_byte errorCode)
  {
    isErrorReported = true;
    EXPECT_EQ(errorCode, M2TP_ERROR_PACKET_SIZE_MISMATCH);
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
  ReceiveBuffer_start(M2TP_COMMAND_ANNOUNCEMENT_JOIN, contentSize);

  // Simulate incomplete receive stream
  for (m2tp_byte i = 0; i < contentSize - 1; i++)
    ReceiveBuffer_write(serializedContent[i]);

  // Simulate finish of receive event
  ReceiveBuffer_finish();

  // Receive interrupt shouldn't be triggered
  EXPECT_FALSE(isReceiveInterrupt);

  // Check if error reported
  EXPECT_TRUE(isErrorReported);

  // Callback reset
  TaskRouter_receiveInterrupt = nullptr;
  TaskRouter_reportReceiveFail = nullptr;
}

TEST(ReceiveBuffer, UninitializedWrite)
{
  // Initialize required global variables
  isReceiveInterrupt = false;
  isErrorReported = false;

  // Prepare test function for receive interrupt
  TaskRouter_receiveInterrupt = [](Packet *packet)
  {
    isReceiveInterrupt = true;
  };

  // Prepare test function for error report
  TaskRouter_reportReceiveFail = [](m2tp_byte errorCode)
  {
    isErrorReported = true;
  };

  // Simulate unitialized write with NULL
  ReceiveBuffer_write(NULL);
  EXPECT_EQ(ReceiveBuffer_position, 0);

  // Simulate another unitialized write
  ReceiveBuffer_write(0x7f);
  EXPECT_EQ(ReceiveBuffer_position, 0);

  // Simulate finish with unitialized condition
  ReceiveBuffer_finish();
  EXPECT_FALSE(isReceiveInterrupt);
  EXPECT_FALSE(isErrorReported);

  // Callback reset
  TaskRouter_receiveInterrupt = nullptr;
  TaskRouter_reportReceiveFail = nullptr;
}
