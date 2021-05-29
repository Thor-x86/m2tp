/**
 * @file PeerTrafficTask.h
 * @brief Core logic of member registration, on leader side
**/

#ifndef IS_PEER_TRAFFIC_TASK_DEFINED
#define IS_PEER_TRAFFIC_TASK_DEFINED

// Will be called by nextTask
extern void PeerTrafficTask_start();

// Will be called by reset
extern void PeerTrafficTask_stop();

#endif // #ifndef IS_PEER_TRAFFIC_TASK_DEFINED
