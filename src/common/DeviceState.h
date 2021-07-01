/**
 * @file DeviceState.h
 * @brief Stores information of current device
**/

#ifndef IS_DEVICE_STATE_DEFINED
#define IS_DEVICE_STATE_DEFINED

#include "m2tp-common/typedef.h"
#include "m2tp-common/base_interface/AppInterface.h"

// Only set from driver side
extern bool DeviceState_driverReady;
extern char *DeviceState_deviceBrand;
extern char *DeviceState_deviceVendor;
extern char *DeviceState_deviceClass;

// NULL means not assigned or as leader
extern m2tp_channel assignedAddress;

// Check if current device is ready to communicate
extern bool DeviceState_isReady();

#endif // #ifndef IS_DEVICE_STATE_DEFINED
