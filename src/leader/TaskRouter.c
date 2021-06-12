/**
 * @file TaskRouter.c
 * @brief Implementation of TaskRouter.h, specifically for leader
**/

#include "../common/TaskRouter.h"

// Available Tasks
#define TASK_REGISTRATION 1
#define TASK_TRANSMIT 2
#define TASK_TRAFFIC_PEER 3
#define TASK_TRAFFIC_BROADCAST 4

void TaskRouter_nextTask()
{
  // TODO: Write stuffs here...
}

void TaskRouter_reset()
{
  TaskRouter_clearFunctions();
  TaskRouter_currentTask = NULL;

  // TODO: Stop all tasks here...
}
