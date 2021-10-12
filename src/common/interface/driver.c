/**
 * @file driver.c
 * @brief Implementation of "include/m2tp/interface/driver.h"
**/

#include "m2tp/interface/driver.h"
#include "m2tp/errors.h"
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

void (*m2tp_driver_onWaitForQueue)() = NULL;

////////////////////////////////////////////////////////

//////// Functions /////////////////////////////////////

void m2tp_driver_setDeviceBrand(const char *value)
{
  DeviceState_deviceBrand = value;
}

void m2tp_driver_setDeviceVendor(const char *value)
{
  DeviceState_deviceVendor = value;
}

void m2tp_driver_setDeviceClass(const char *value)
{
  DeviceState_deviceClass = value;
}

void m2tp_driver_connected()
{
  if (!DeviceState_driverReady)
  {
    DeviceState_driverReady = true;
    TaskRouter_start();
  }
}

void m2tp_driver_disconnected()
{
  if (DeviceState_driverReady)
  {
    DeviceState_driverReady = false;
    TaskRouter_stop();
  }
}

bool m2tp_driver_receiveStart(
    m2tp_byte command,
    m2tp_byte contentSize)
{
  // Abort if driver NOT ready yet
  if (!DeviceState_driverReady)
    return false;

  return ReceiveBuffer_start(command, contentSize);
}

m2tp_byte m2tp_driver_receiveWrite(m2tp_byte value)
{
  // Abort if driver NOT ready yet
  if (!DeviceState_driverReady)
    return 0;

  return ReceiveBuffer_write(value);
}

void m2tp_driver_receiveFragmentEnd()
{
  // Abort if driver NOT ready yet
  if (!DeviceState_driverReady)
    return;

  TaskRouter_restartTimeout();
}

m2tp_error m2tp_driver_receiveEnd()
{
  // Abort if driver NOT ready yet
  if (!DeviceState_driverReady)
    return M2TP_ERROR_NOT_CONNECTED;

  return ReceiveBuffer_finish();
}

////////////////////////////////////////////////////////
