/**
 * @file NetworkState.h
 * @brief Stores information about the network
**/

#ifndef IS_NETWORK_STATE_DEFINED
#define IS_NETWORK_STATE_DEFINED

#include "m2tp/typedef.h"

// Holds really big number of flags (128 flags) with
// each flag represents an address currently assigned (1) or still vacant (0)
extern volatile unsigned long long NetworkState_addressRegistry[2];

// Similar to `NetworkState_addressRegistry` but for topic ID
extern volatile unsigned long long NetworkState_topicRegistry[2];

// If the value equals 1, then echo every incoming transmit packets
extern volatile m2tp_byte NetworkState_isEcho;

// List of assigned device classes,
// The index is device address and its value is device class
//
// WARNING: This array's elements utilize heap memory except index 0.
//          You MUST USE free() from stdlib.h to clean them!
extern volatile char *NetworkState_deviceClasses[128];

// List of assigned topic names,
// The index is topic ID and its value is topic name
//
// WARNING: This array's elements utilize heap memory.
//          You MUST USE free() from stdlib.h to clean them!
extern volatile char *NetworkState_topicNames[128];

// Everytime an address assigned, leader need to
// plan next vacant address to save time
extern volatile m2tp_channel NetworkState_nextVacantAddress;

// Same as `nextVacantAddress` but for topic ID
extern volatile m2tp_channel NetworkState_nextVacantTopicID;

// Initialize all arrays of strings
extern void NetworkState_init();

// Marks address or topic ID as assigned
extern void NetworkState_assign(m2tp_channel, const char *identity, unsigned int identitySize);

// Marks address or topic ID as vacant (or not in use)
extern void NetworkState_unassign(m2tp_channel);

// Check if address or topic ID assigned or not
extern bool NetworkState_isAssigned(m2tp_channel);

// Find device address based on its class
extern m2tp_channel NetworkState_findDevice(const char *deviceClass);

// Find topic ID based on its name
extern m2tp_channel NetworkState_findTopic(const char *topicName);

// Automatically looking for next vacant device address
extern void NetworkState_resolveNextVacantAddress();

// Automatically looking for next vacant topic ID
extern void NetworkState_resolveNextVacantTopicID();

// Clean RAM usage, will be called when driver request for disconnect
extern void NetworkState_reset();

// Syntactic sugar for checking if all available addresses are in use
#define NetworkState_isAddressFull ((NetworkState_addressRegistry[0] == 0xFFFFFFFFFFFFFFFFULL) && (NetworkState_addressRegistry[1] == 0xFFFFFFFFFFFFFFFFULL))

// Syntactic sugar for checking if all available topics are in use
#define NetworkState_isTopicFull ((NetworkState_topicRegistry[0] == 0xFFFFFFFFFFFFFFFFULL) && (NetworkState_topicRegistry[1] == 0xFFFFFFFFFFFFFFFFULL))

#endif // #ifndef IS_NETWORK_STATE_DEFINED
