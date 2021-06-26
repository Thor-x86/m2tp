/**
 * @file TaskRouter.c
 * @brief Implementation of TaskRouter.h, specifically for member
**/

#include "../common/TaskRouter.h"

#include "m2tp-common/commands.h"
#include "task/RegistrationTask.h"
#include "task/MainTask.h"

// Available Tasks
#define TASK_REGISTRATION 1
#define TASK_MAIN 2

void TaskRouter_start()
{
  // TODO: Write stuffs here...
}

void TaskRouter_nextTask()
{
  // TODO: Write stuffs here...
}

void TaskRouter_stop()
{
  // TODO: Write stuffs here...
}

void TaskRouter_sendPacket(Packet *packet)
{
  // Making sure packet isn't NULL
  // to prevent further damage
  if (packet == NULL)
    return;

  // Member can only send "data transmit" packet type,
  // otherwise it will be ignored
  if (packet->command == M2TP_COMMAND_TRANSMIT)
    MainTask_send(packet);
}

bool TaskRouter_hasPendingData()
{
  return MainTask_hasPendingData();
}
