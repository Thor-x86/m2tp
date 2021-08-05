/**
 * @file NetworkState.test.cpp
 * @brief Unit test for NetworkState.h
**/

#include "gtest/gtest.h"

extern "C"
{
#include "NetworkState.h"
}

TEST(NetworkState, DefaultValues)
{
  EXPECT_EQ(NetworkState_addressRegistry[0], 0);
  EXPECT_EQ(NetworkState_addressRegistry[1], 1);
  EXPECT_EQ(NetworkState_isEcho, 0);
  EXPECT_EQ(NetworkState_topicRegistry[0], 0);
  EXPECT_EQ(NetworkState_topicRegistry[1], 0);
  EXPECT_EQ(NetworkState_nextVacantAddress, 1);
  EXPECT_EQ(NetworkState_nextVacantTopicID, 0);
}

TEST(NetworkState, AddressRegistry)
{
  ASSERT_EQ(NetworkState_addressRegistry[0], 0) << "Invalid initial value, cannot continue";
  ASSERT_EQ(NetworkState_addressRegistry[1], 1) << "Invalid initial value, cannot continue";

  NetworkState_assign(13);
  NetworkState_assign(27);
  NetworkState_assign(103);
  NetworkState_assign(118);
  NetworkState_unassign(27);
  NetworkState_unassign(33);
  NetworkState_unassign(118);
  NetworkState_unassign(125);
  NetworkState_assign(13);
  NetworkState_assign(103);

  ASSERT_EQ(NetworkState_addressRegistry[1], 0x2001ULL);
  ASSERT_EQ(NetworkState_addressRegistry[0], 0x8000000000ULL);

  EXPECT_TRUE(NetworkState_isAssigned(13));
  EXPECT_TRUE(NetworkState_isAssigned(103));
  EXPECT_FALSE(NetworkState_isAssigned(27));
  EXPECT_FALSE(NetworkState_isAssigned(33));
  EXPECT_FALSE(NetworkState_isAssigned(118));
  EXPECT_FALSE(NetworkState_isAssigned(125));

  // Cleanup
  NetworkState_addressRegistry[0] = 0;
  NetworkState_addressRegistry[1] = 1;
}

TEST(NetworkState, TopicRegistry)
{
  ASSERT_EQ(NetworkState_topicRegistry[0], 0) << "Invalid initial value, cannot continue";
  ASSERT_EQ(NetworkState_topicRegistry[1], 0) << "Invalid initial value, cannot continue";

  NetworkState_assign(141);
  NetworkState_assign(155);
  NetworkState_assign(231);
  NetworkState_assign(246);
  NetworkState_unassign(155);
  NetworkState_unassign(161);
  NetworkState_unassign(246);
  NetworkState_unassign(253);
  NetworkState_assign(141);
  NetworkState_assign(231);

  ASSERT_EQ(NetworkState_topicRegistry[1], 0x2000ULL);
  ASSERT_EQ(NetworkState_topicRegistry[0], 0x8000000000ULL);

  EXPECT_TRUE(NetworkState_isAssigned(141));
  EXPECT_TRUE(NetworkState_isAssigned(231));
  EXPECT_FALSE(NetworkState_isAssigned(155));
  EXPECT_FALSE(NetworkState_isAssigned(161));
  EXPECT_FALSE(NetworkState_isAssigned(246));
  EXPECT_FALSE(NetworkState_isAssigned(253));

  // Cleanup
  NetworkState_topicRegistry[0] = 0;
  NetworkState_topicRegistry[1] = 0;
}

TEST(NetworkState, FindTopic)
{
  ASSERT_EQ(NetworkState_topicRegistry[0], 0) << "Invalid initial value, cannot continue";
  ASSERT_EQ(NetworkState_topicRegistry[1], 0) << "Invalid initial value, cannot continue";

  ASSERT_EQ(NetworkState_topicNames[131 - 128], nullptr) << "Topic name with ID 131 is not empty, cannot continue";
  EXPECT_EQ(NetworkState_findTopic("Lorem Ipsum"), NULL) << "findTopic() function broken, cannot continue";

  // Topic name example
  const char *topicName = "Lorem Ipsum";

  // Assign topic name
  NetworkState_assign(131);
  NetworkState_topicNames[131 - 128] = (char *)malloc(12);
  memcpy((void *)NetworkState_topicNames[131 - 128], topicName, 12);

  // Check if it's working
  EXPECT_EQ(NetworkState_findTopic("Lorem Ipsum"), 131);

  // Cleanup
  free((void *)NetworkState_topicNames[131 - 128]);
  NetworkState_topicRegistry[0] = 0;
  NetworkState_topicRegistry[1] = 0;
}
