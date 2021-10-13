/**
 * @file NetworkState.c
 * @brief Implementation of NetworkState.h
**/

#include "NetworkState.h"

#include "../common/DeviceState.h"

#include <stdlib.h>
#include <string.h>

//////// Public Variables //////////////////////////////

volatile unsigned long long NetworkState_addressRegistry[2] = {0ULL, 1ULL};
volatile unsigned long long NetworkState_topicRegistry[2] = {0ULL, 0ULL};
volatile m2tp_byte NetworkState_isEcho = 0;
volatile m2tp_channel NetworkState_nextVacantAddress = 1;
volatile m2tp_channel NetworkState_nextVacantTopicID = 0;
volatile char *NetworkState_deviceClasses[128];
volatile char *NetworkState_topicNames[128];

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void NetworkState_init()
{
  // Initialize device classes and topic names
  memset(NetworkState_deviceClasses, 0, sizeof(NetworkState_deviceClasses));
  memset(NetworkState_topicNames, 0, sizeof(NetworkState_topicNames));

  // Register leader's class
  NetworkState_deviceClasses[0] = (volatile char *)DeviceState_deviceClass;
}

void NetworkState_assign(
    m2tp_channel channel,
    const char *identity,
    unsigned int identitySize)
{
  // Channel is device address
  if (channel == 0)
    return; // Channel 0 is reserved for leader
  else if (channel < 64)
    NetworkState_addressRegistry[1] |= (1ULL << channel);
  else if (channel < 128)
    NetworkState_addressRegistry[0] |= (1ULL << (channel - 64));

  // Channel is topic ID
  else if (channel < 192)
    NetworkState_topicRegistry[1] |= (1ULL << (channel - 128));
  else
    NetworkState_topicRegistry[0] |= (1ULL << (channel - 192));

  // Copy the identity to heap memory
  char *heapIdentity = malloc(identitySize);
  memcpy(heapIdentity, identity, identitySize);

  // Channel is device address, then identity is Device Class
  if (channel < 128)
  {
    if (NetworkState_deviceClasses[channel] != NULL)
      free((void *)NetworkState_deviceClasses[channel]);

    NetworkState_deviceClasses[channel] = heapIdentity;
  }

  // Channel is topic ID, then identity is Topic Name
  else
  {
    m2tp_channel index = channel - 128;

    if (NetworkState_topicNames[index] != NULL)
      free((void *)NetworkState_topicNames[index]);

    NetworkState_topicNames[index] = heapIdentity;
  }
}

void NetworkState_unassign(m2tp_channel channel)
{
  // Channel is device address
  if (channel == 0)
    return; // Channel 0 is reserved for leader
  else if (channel < 64)
    NetworkState_addressRegistry[1] &= ~(1ULL << channel);
  else if (channel < 128)
    NetworkState_addressRegistry[0] &= ~(1ULL << (channel - 64));

  // Channel is topic ID
  else if (channel < 192)
    NetworkState_topicRegistry[1] &= ~(1ULL << (channel - 128));
  else
    NetworkState_topicRegistry[0] &= ~(1ULL << (channel - 192));

  // Channel is device address, then identity is Device Class
  if (channel < 128)
  {
    if (NetworkState_deviceClasses[channel] != NULL)
    {
      free((void *)NetworkState_deviceClasses[channel]);
      NetworkState_deviceClasses[channel] = NULL;
    }
  }

  // Channel is topic ID, then identity is Topic Name
  else
  {
    m2tp_channel index = channel - 128;
    if (NetworkState_topicNames[index] != NULL)
    {
      free((void *)NetworkState_topicNames[index]);
      NetworkState_topicNames[index] = NULL;
    }
  }
}

bool NetworkState_isAssigned(m2tp_channel channel)
{
  // Channel is device address
  if (channel < 64)
    return (NetworkState_addressRegistry[1] >> channel) & 1ULL;
  else if (channel < 128)
  {
    channel -= 64;
    return (NetworkState_addressRegistry[0] >> channel) & 1ULL;
  }

  // Channel is topic ID
  else if (channel < 192)
  {
    channel -= 128;
    return (NetworkState_topicRegistry[1] >> channel) & 1ULL;
  }
  else
  {
    channel -= 192;
    return (NetworkState_topicRegistry[0] >> channel) & 1ULL;
  }
}

m2tp_channel NetworkState_findDevice(const char *deviceClass)
{
  if (deviceClass == NULL)
    return 0;

  m2tp_channel deviceAddress = 0;
  while (deviceAddress < 128)
  {
    if (NetworkState_isAssigned(deviceAddress))
    {
      for (m2tp_byte i = 0; i < 254; i++)
      {
        if (NetworkState_deviceClasses[deviceAddress][i] == deviceClass[i])
        {
          if (deviceClass[i] == '\0')
            return deviceAddress;
        }
        else
          break;
      }
    }
    deviceAddress++;
  }

  return 0;
}

m2tp_channel NetworkState_findTopic(const char *topicName)
{
  if (topicName == NULL)
    return 0;

  m2tp_channel topicID = 128;

  // When topicID exceed maximum size,
  // it turn itself to be device address.
  // Thus we have to stop this "while" loop when it happens.
  while ((topicID & 0b10000000U) == 0b10000000U)
  {
    if (NetworkState_isAssigned(topicID))
    {
      for (m2tp_byte i = 0; i < 254; i++)
      {
        if (NetworkState_topicNames[topicID - 128][i] == topicName[i])
        {
          if (topicName[i] == '\0')
            return topicID;
        }
        else
          break;
      }
    }
    topicID++;
  }

  return 0;
}

void NetworkState_resolveNextVacantAddress()
{
  // Is address NOT full?
  if (!NetworkState_isAddressFull)
  {
    do
    {
      // Set the next vacant address
      NetworkState_nextVacantAddress++;

      // nextVacantAddress too big?
      if (NetworkState_nextVacantAddress >= 128U)
      {
        // Reset nextVacantAddress
        NetworkState_nextVacantAddress = 1;
      }

      // Is nextVacantAddress already assigned? if yes then repeat
    } while (NetworkState_isAssigned(NetworkState_nextVacantAddress));
  }
}

void NetworkState_resolveNextVacantTopicID()
{
  // Is topic NOT full?
  if (!NetworkState_isTopicFull)
  {
    do
    {
      // Set the next vacant topic ID
      NetworkState_nextVacantTopicID++;

      // nextVacantTopicID exceed maximum limit?
      if ((NetworkState_nextVacantTopicID & 0b10000000U) != 0b10000000U)
      {
        // Reset nextVacantTopicID
        NetworkState_nextVacantTopicID = 128;
      }

      // Is nextVacantTopicID already assigned? if yes then repeat
    } while (NetworkState_isAssigned(NetworkState_nextVacantTopicID));
  }
}

void NetworkState_reset()
{
  for (unsigned int channel = 1; channel < 256; channel++)
    NetworkState_unassign(channel);
}

////////////////////////////////////////////////////////
