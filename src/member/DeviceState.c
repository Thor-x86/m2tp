/**
 * @file DeviceState.c
 * @brief Implementation of DeviceState.h, special for member device
**/

#include "../common/DeviceState.h"

bool DeviceState_isReady()
{
  return DeviceState_driverReady && (DeviceState_assignedAddress > 0);
}
