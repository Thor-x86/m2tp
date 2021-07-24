/**
 * @file NetworkState.c
 * @brief Implementation of NetworkState.h
**/

#include "NetworkState.h"

#include <stdlib.h>

//////// Public Variables //////////////////////////////

volatile unsigned long long NetworkState_addressRegistry[2] = {0, 1};
volatile unsigned long long NetworkState_topicRegistry[2] = {0, 0};
volatile m2tp_channel NetworkState_nextVacantAddress = 1;
volatile m2tp_channel NetworkState_nextVacantTopicID = 0;
volatile char *NetworkState_topicNames[128];

////////////////////////////////////////////////////////

//////// Public Functions //////////////////////////////

void NetworkState_deleteTopicName(m2tp_channel topicID)
{
  m2tp_byte index = topicID - 128;

  if (NetworkState_isAssigned(topicID) && NetworkState_topicNames[index] != NULL)
  {
    free(NetworkState_topicNames[index]);
    NetworkState_topicNames[index] = NULL;
  }
}

void NetworkState_assign(m2tp_channel channel)
{
  // Channel is device address
  if (channel < 64)
    NetworkState_addressRegistry[1] |= (1ULL << channel);
  else if (channel < 128)
    NetworkState_addressRegistry[0] |= (1ULL << (channel - 64));

  // Channel is topic ID
  else if (channel < 192)
    NetworkState_topicRegistry[1] |= (1ULL << (channel - 128));
  else
    NetworkState_topicRegistry[0] |= (1ULL << (channel - 192));
}

void NetworkState_unassign(m2tp_channel channel)
{
  // Channel is device address
  if (channel < 64)
    NetworkState_addressRegistry[1] &= ~(1ULL << channel);
  else if (channel < 128)
    NetworkState_addressRegistry[0] &= ~(1ULL << (channel - 64));

  // Channel is topic ID
  else if (channel < 192)
    NetworkState_topicRegistry[1] &= ~(1ULL << (channel - 128));
  else
    NetworkState_topicRegistry[0] &= ~(1ULL << (channel - 192));
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

m2tp_byte NetworkState_findTopic(char *topicName)
{
  if (topicName == NULL)
    return 0;

  m2tp_byte topicID = 128;

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
  // Delete all topic names from RAM
  for (m2tp_byte topicID = 128; topicID >= 128; topicID++)
    NetworkState_deleteTopicName(topicID);

  // Reset registry
  NetworkState_addressRegistry[0] = 0ULL;
  NetworkState_addressRegistry[1] = 0ULL;
  NetworkState_topicRegistry[0] = 0ULL;
  NetworkState_topicRegistry[1] = 0ULL;
}

////////////////////////////////////////////////////////
