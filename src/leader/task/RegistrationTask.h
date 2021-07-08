/**
 * @file RegistrationTask.h
 * @brief Ask to anyone whom not registered yet
**/

#ifndef IS_REGISTRATION_TASK_DEFINED
#define IS_REGISTRATION_TASK_DEFINED

#include "../../common/packet/Packet.h"

// Mandatory function, this will attach private functions
// to TaskRouter then initialize stuffs
extern void RegistrationTask_start();

// Mandatory function, this will detach functions
// from TaskRouter then do cleanups
extern void RegistrationTask_stop();

#endif // #ifndef IS_REGISTRATION_TASK_DEFINED
