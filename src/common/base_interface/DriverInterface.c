/**
 * @file DriverInterface.c
 * @brief Implementation of "m2tp-common/base_interface/DriverInterface.h"
**/

#include "m2tp-common/base_interface/DriverInterface.h"
#include "../DeviceState.h"
#include "../packet/Packet.h"
#include "../TaskRouter.h"
#include "../buffer/ReceiveBuffer.h"

//////// Variables /////////////////////////////////////

void (*m2tp_driver_sendListener)(
    m2tp_byte command,
    m2tp_byte packetSize,
    const m2tp_bytes rawData) = NULL;

void (*m2tp_driver_startTimerListener)(
    unsigned long int duration,
    m2tp_driver_TimerFinishCallback callback) = NULL;

void (*m2tp_driver_stopTimerListener)() = NULL;

////////////////////////////////////////////////////////

//////// Functions /////////////////////////////////////

void m2tp_driver_setDeviceBrand(char *value)
{
  DeviceState_deviceBrand = value;
}

void m2tp_driver_setDeviceVendor(char *value)
{
  DeviceState_deviceVendor = value;
}

void m2tp_driver_setDeviceClass(char *value)
{
  DeviceState_deviceClass = value;
}

void m2tp_driver_connected()
{
  DeviceState_driverReady = true;
  TaskRouter_start();
}

void m2tp_driver_disconnected()
{
  TaskRouter_stop();
  DeviceState_driverReady = false;
}

void m2tp_driver_receiveStart(
    m2tp_byte command,
    m2tp_byte contentSize)
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return;

  ReceiveBuffer_start(command, contentSize);
}

void m2tp_driver_receiveWrite(m2tp_byte value)
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return;

  ReceiveBuffer_write(value);
}

void m2tp_driver_receiveEnd()
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return;

  ReceiveBuffer_finish();
}

////////////////////////////////////////////////////////
