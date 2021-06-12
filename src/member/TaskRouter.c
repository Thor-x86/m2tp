/**
 * @file TaskRouter.c
 * @brief Implementation of TaskRouter.h, specifically for member
**/

#include "../common/TaskRouter.h"

// Available Tasks
#define TASK_REGISTRATION 1
#define TASK_TRANSMIT 2

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
