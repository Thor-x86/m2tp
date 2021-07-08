/**
 * @file DeviceState.h
 * @brief Stores information of current device
**/

#ifndef IS_DEVICE_STATE_DEFINED
#define IS_DEVICE_STATE_DEFINED

#include "m2tp-common/typedef.h"
#include "m2tp-common/base_interface/AppInterface.h"

// Only set from driver side
extern bool DeviceState_driverReady;
extern char *DeviceState_deviceBrand;
extern char *DeviceState_deviceVendor;
extern char *DeviceState_deviceClass;

// NULL means not assigned or as leader
extern m2tp_channel DeviceState_assignedAddress;

// Key-value map for "(TopicID-128) => Listener"
extern m2tp_TopicListener DeviceState_subscribedTopics[128];

// Clear and initialize topic subscription
extern void DeviceState_resetTopics();

// Check if current device is ready to communicate
extern bool DeviceState_isReady();

// Syntactic sugar for subscribing a listener function to specific topicID
#define DeviceState_assignTopic(topicID, listener) (DeviceState_subscribedTopics[topicID - 128U] = listener)

// Syntactic sugar for unsubscribe a topic
#define DeviceState_unassignTopic(topicID) (DeviceState_subscribedTopics[topicID - 128U] = NULL)

// Syntactic sugar for checking if topic is assigned or not
#define DeviceState_isTopicAssigned(topicID) (DeviceState_subscribedTopics[topicID - 128U] != NULL)

// Syntactic sugar for calling topic listener
#define DeviceState_callTopicListener(topicID, dataSize, data) (DeviceState_subscribedTopics[topicID - 128U](dataSize, data))

#endif // #ifndef IS_DEVICE_STATE_DEFINED
