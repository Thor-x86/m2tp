/**
 * @file DriverInterface.c
 * @brief Implementation of "m2tp-common/base_interface/DriverInterface.h"
**/

#include "m2tp-common/base_interface/DriverInterface.h"
#include "../DeviceState.h"
#include "../packet/Packet.h"
#include "../TaskRouter.h"

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
  deviceBrand = value;
}

void m2tp_driver_setDeviceVendor(char *value)
{
  deviceVendor = value;
}

void m2tp_driver_setDeviceClass(char *value)
{
  deviceClass = value;
}

void m2tp_driver_connected()
{
  driverReady = true;
  TaskRouter_nextTask();
}

void m2tp_driver_disconnected()
{
  driverReady = false;
  assignedAddress = false;
  TaskRouter_reset();
}

void m2tp_driver_receiveStart(
    m2tp_byte command,
    m2tp_byte contentSize)
{
  // TODO: Write stuffs here...
}

void m2tp_driver_receiveWrite(m2tp_byte value)
{
  // TODO: Write stuffs here...
}

void m2tp_driver_receiveEnd()
{
  // TODO: Write stuffs here...
}

void m2tp_driver_receiveFailed(m2tp_byte errorCode)
{
  if (TaskRouter_reportReceiveFail != NULL)
    TaskRouter_reportReceiveFail(errorCode);
}

void m2tp_driver_sendFailed(m2tp_byte errorCode)
{
  if (TaskRouter_reportSendFail != NULL)
    TaskRouter_reportSendFail(errorCode);
}

////////////////////////////////////////////////////////
