/**
 * @file TrafficTask.h
 * @brief Controls how members transmit their data
**/

#ifndef IS_TRAFFIC_TASK_DEFINED
#define IS_TRAFFIC_TASK_DEFINED

#include "../../common/packet/Packet.h"

// Mandatory function, this will attach private functions
// to TaskRouter then initialize stuffs
extern void TrafficTask_start();

// Mandatory function, this will detach functions
// from TaskRouter then do cleanups
extern void TrafficTask_stop();

#endif // #ifndef IS_TRAFFIC_TASK_DEFINED
