/**
 * @file DeviceState.c
 * @brief Generic implementation of DeviceState.h
**/

#include "DeviceState.h"

bool DeviceState_driverReady = false;
char *DeviceState_deviceBrand = "generic";
char *DeviceState_deviceVendor = "generic";
char *DeviceState_deviceClass = "generic";
m2tp_channel DeviceState_assignedAddress = NULL;
// Variables:
bool driverReady = false;
char *deviceBrand = "generic";
char *deviceVendor = "generic";
char *deviceClass = "generic";
m2tp_channel assignedAddress = NULL;

// NOTE: Other functions are implemented at member and leader
//       because they're work differently
