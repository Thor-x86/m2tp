/**
 * @file DeviceState.c
 * @brief Implementation of DeviceState.h
**/

#include "DeviceState.h"

// Variables:
bool driverReady = false;
char *deviceBrand = "generic";
char *deviceVendor = "generic";
char *deviceClass = "generic";
m2tp_channel assignedAddress = NULL;

bool isReady()
{
  return driverReady && assignedAddress != NULL;
}
