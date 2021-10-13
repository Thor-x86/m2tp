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
  EXPECT_EQ(NetworkState_addressRegistry[0], 0ULL);
  EXPECT_EQ(NetworkState_addressRegistry[1], 1ULL);
  EXPECT_EQ(NetworkState_isEcho, 0);
  EXPECT_EQ(NetworkState_topicRegistry[0], 0ULL);
  EXPECT_EQ(NetworkState_topicRegistry[1], 0ULL);
  EXPECT_EQ(NetworkState_nextVacantAddress, 1);
  EXPECT_EQ(NetworkState_nextVacantTopicID, 0);
}

TEST(NetworkState, AddressRegistry)
{
  ASSERT_EQ(NetworkState_addressRegistry[0], 0ULL) << "Invalid initial value, cannot continue";
  ASSERT_EQ(NetworkState_addressRegistry[1], 1ULL) << "Invalid initial value, cannot continue";

  NetworkState_assign(13, "Example Device #1", sizeof("Example Device #1"));
  NetworkState_assign(27, "Example Device #2", sizeof("Example Device #2"));
  NetworkState_assign(103, "Example Device #3", sizeof("Example Device #3"));
  NetworkState_assign(118, "Example Device #4", sizeof("Example Device #4"));
  NetworkState_unassign(27);
  NetworkState_unassign(33);
  NetworkState_unassign(118);
  NetworkState_unassign(125);
  NetworkState_assign(13, "Example Device #5", sizeof("Example Device #5"));
  NetworkState_assign(103, "Example Device #6", sizeof("Example Device #6"));

  ASSERT_EQ(NetworkState_addressRegistry[1], 0x2001ULL);
  ASSERT_EQ(NetworkState_addressRegistry[0], 0x8000000000ULL);

  EXPECT_TRUE(NetworkState_isAssigned(13));
  EXPECT_TRUE(NetworkState_isAssigned(103));
  EXPECT_FALSE(NetworkState_isAssigned(27));
  EXPECT_FALSE(NetworkState_isAssigned(33));
  EXPECT_FALSE(NetworkState_isAssigned(118));
  EXPECT_FALSE(NetworkState_isAssigned(125));

  EXPECT_STREQ((char *)NetworkState_deviceClasses[13], "Example Device #5");
  EXPECT_STREQ((char *)NetworkState_deviceClasses[103], "Example Device #6");
  EXPECT_EQ((char *)NetworkState_deviceClasses[27], nullptr);
  EXPECT_EQ((char *)NetworkState_deviceClasses[33], nullptr);
  EXPECT_EQ((char *)NetworkState_deviceClasses[118], nullptr);
  EXPECT_EQ((char *)NetworkState_deviceClasses[125], nullptr);

  // Cleanup
  NetworkState_unassign(13);
  NetworkState_unassign(103);
}

TEST(NetworkState, TopicRegistry)
{
  ASSERT_EQ(NetworkState_topicRegistry[0], 0ULL) << "Invalid initial value, cannot continue";
  ASSERT_EQ(NetworkState_topicRegistry[1], 0ULL) << "Invalid initial value, cannot continue";

  NetworkState_assign(141, "Example Topic #1", sizeof("Example Topic #1"));
  NetworkState_assign(155, "Example Topic #2", sizeof("Example Topic #2"));
  NetworkState_assign(231, "Example Topic #3", sizeof("Example Topic #3"));
  NetworkState_assign(246, "Example Topic #4", sizeof("Example Topic #4"));
  NetworkState_unassign(155);
  NetworkState_unassign(161);
  NetworkState_unassign(246);
  NetworkState_unassign(253);
  NetworkState_assign(141, "Example Topic #5", sizeof("Example Topic #5"));
  NetworkState_assign(231, "Example Topic #6", sizeof("Example Topic #6"));

  ASSERT_EQ(NetworkState_topicRegistry[1], 0x2000ULL);
  ASSERT_EQ(NetworkState_topicRegistry[0], 0x8000000000ULL);

  EXPECT_TRUE(NetworkState_isAssigned(141));
  EXPECT_TRUE(NetworkState_isAssigned(231));
  EXPECT_FALSE(NetworkState_isAssigned(155));
  EXPECT_FALSE(NetworkState_isAssigned(161));
  EXPECT_FALSE(NetworkState_isAssigned(246));
  EXPECT_FALSE(NetworkState_isAssigned(253));

  // Cleanup
  NetworkState_topicRegistry[0] = 0ULL;
  NetworkState_topicRegistry[1] = 0ULL;
}

TEST(NetworkState, FindTopic)
{
  ASSERT_EQ(NetworkState_topicRegistry[0], 0ULL) << "Invalid initial value, cannot continue";
  ASSERT_EQ(NetworkState_topicRegistry[1], 0ULL) << "Invalid initial value, cannot continue";

  ASSERT_EQ(NetworkState_topicNames[131 - 128], nullptr) << "Topic name with ID 131 is not empty, cannot continue";
  ASSERT_EQ(NetworkState_findTopic("Lorem Ipsum"), NULL) << "findTopic() function broken, cannot continue";

  // Topic name example
  const char *topicName = "Lorem Ipsum";
  size_t topicNameSize = sizeof("Lorem Ipsum");

  // Assign topic name
  NetworkState_assign(131, topicName, topicNameSize);

  // Check if it's working
  EXPECT_EQ(NetworkState_findTopic(topicName), 131);

  // Cleanup
  NetworkState_unassign(131);
}
