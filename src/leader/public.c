/**
 * @file public.c
 * @brief Implementation of "include/m2tp-leader.h"
 **/

#include "m2tp-leader.h"

#include "./NetworkState.h"

#include "../common/DeviceState.h"

//////// Variables /////////////////////////////////////

// TODO: Write stuffs here...

////////////////////////////////////////////////////////

//////// Functions /////////////////////////////////////

void m2tp_enableEchoTransmit(bool value)
{
  if (value)
    NetworkState_isEcho = 1;
  else
    NetworkState_isEcho = 0;
}

const char *m2tp_getMemberDeviceClass(m2tp_channel deviceAddress)
{
  if (!DeviceState_driverReady)
    return NULL;

  return (const char *)NetworkState_deviceClasses[deviceAddress];
}

const char *m2tp_getTopicName(m2tp_channel topicID)
{
  if (!DeviceState_driverReady)
    return NULL;

  return (const char *)NetworkState_topicNames[topicID];
}

m2tp_channel m2tp_findMemberAddress(const char *deviceClass)
{
  if (!DeviceState_driverReady)
    return 0;

  return NetworkState_findDevice(deviceClass);
}

m2tp_channel m2tp_findTopicID(const char *topicName)
{
  if (!DeviceState_driverReady)
    return 0;

  return NetworkState_findTopic(topicName);
}

////////////////////////////////////////////////////////
