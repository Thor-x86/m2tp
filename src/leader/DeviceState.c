/**
 * @file DeviceState.c
 * @brief Implementation of DeviceState.h, special for leader device
**/

#include "../common/DeviceState.h"

bool DeviceState_isReady()
{
  return DeviceState_driverReady;
}
