/**
 * @file BroadcastTrafficTask.h
 * @brief Core logic of member registration, on leader side
**/

#ifndef IS_BROADCAST_TRAFFIC_TASK_DEFINED
#define IS_BROADCAST_TRAFFIC_TASK_DEFINED

// Will be called by nextTask
extern void BroadcastTrafficTask_start();

// Will be called by reset
extern void BroadcastTrafficTask_stop();

#endif // #ifndef IS_BROADCAST_TRAFFIC_TASK_DEFINED
