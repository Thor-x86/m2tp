/**
 * @file RegistrationTask.h
 * @brief Core logic of member registration, on member side
**/

#ifndef IS_REGISTRATION_TASK_DEFINED
#define IS_REGISTRATION_TASK_DEFINED

// Will be called by nextTask
extern void RegistrationTask_start();

// Will be called by reset
extern void RegistrationTask_stop();

#endif // #ifndef IS_REGISTRATION_TASK_DEFINED
