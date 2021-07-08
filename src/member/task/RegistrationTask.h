/**
 * @file RegistrationTask.h
 * @brief This task registers current device to the network
 * @note Only run when the device isn't registered yet
**/

#ifndef IS_REGISTRATION_TASK_DEFINED
#define IS_REGISTRATION_TASK_DEFINED

#include "m2tp-common/base_interface/AppInterface.h"

#include "../../common/packet/Packet.h"

// Mandatory function, this will attach private functions
// to TaskRouter then initialize stuffs
extern void RegistrationTask_start();

// Mandatory function, this will detach functions
// from TaskRouter then do cleanups
extern void RegistrationTask_stop();

// Put topic name here to request for subscribe that topic
extern char *RegistrationTask_pendingTopicName;

// Pointer to a function, will be called on topic created
extern m2tp_OnRegisteredCallback RegistrationTask_pendingTopicOnSuccess;

// Pointer to a function, will be called on failure
extern m2tp_OnErrorCallback RegistrationTask_pendingTopicOnFailed;

// Put pointer to a function here for assign it with topic name
// at `RegistrationTask_pendingSubscribeName` variable
extern m2tp_TopicListener RegistrationTask_pendingTopicListener;

#endif // #ifndef IS_REGISTRATION_TASK_DEFINED
