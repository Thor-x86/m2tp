/**
 * @file DeviceState.h
 * @brief Stores information of current device
**/

#ifndef IS_DEVICE_STATE_DEFINED
#define IS_DEVICE_STATE_DEFINED

#include "m2tp-common/typedef.h"

// Only set from driver side
extern bool driverReady;
extern char *deviceBrand;
extern char *deviceVendor;
extern char *deviceClass;

// NULL means not assigned or as leader
extern m2tp_channel assignedAddress;

// Check if current device is ready to communicate
extern bool isReady();

#endif // #ifndef IS_DEVICE_STATE_DEFINED
