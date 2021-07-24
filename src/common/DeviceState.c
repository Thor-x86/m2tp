/**
 * @file DeviceState.c
 * @brief Generic implementation of DeviceState.h
**/

#include "DeviceState.h"

volatile bool DeviceState_driverReady = false;
char *DeviceState_deviceBrand = "generic";
char *DeviceState_deviceVendor = "generic";
char *DeviceState_deviceClass = "generic";
volatile m2tp_channel DeviceState_assignedAddress = 0;
volatile m2tp_TopicListener DeviceState_subscribedTopics[128];

void DeviceState_resetTopics()
{
  for (m2tp_byte i = 0; i < 128; i++)
    DeviceState_subscribedTopics[i] = NULL;
}

// NOTE: Other functions are implemented at member and leader
//       because they're work differently
