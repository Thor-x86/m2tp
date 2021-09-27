/**
 * @file app.c
 * @brief Implementation of "include/m2tp/interface/app.h"
**/

#include "m2tp/interface/app.h"
#include "m2tp/errors.h"
#include "../DeviceState.h"
#include "../TaskRouter.h"
#include "../buffer/TransmitBuffer.h"

//////// Variables /////////////////////////////////////

void (*m2tp_receivedListener)(
    m2tp_channel sourceAddress,
    m2tp_byte dataSize,
    const m2tp_bytes data) = NULL;

void (*m2tp_onNewMemberListener)(
    char *memberClass,
    m2tp_channel assignedAddress) = NULL;

void (*m2tp_onAnotherMemberQuitListener)(
    m2tp_channel assignedAddress) = NULL;

////////////////////////////////////////////////////////

//////// Functions /////////////////////////////////////

m2tp_error m2tp_startSend(m2tp_channel targetAddress)
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return M2TP_ERROR_NOT_CONNECTED;

  return TransmitBuffer_startPeer(targetAddress);
}

m2tp_error m2tp_startBroadcast(m2tp_channel topicID)
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return M2TP_ERROR_NOT_CONNECTED;

  return TransmitBuffer_startBroadcast(topicID);
}

m2tp_error m2tp_write(m2tp_byte eachByte)
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return M2TP_ERROR_NOT_CONNECTED;

  return TransmitBuffer_write(eachByte);
}

m2tp_error m2tp_writeFinish()
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return M2TP_ERROR_NOT_CONNECTED;

  return TransmitBuffer_finish();
}

void m2tp_writeFinishAsync(
    m2tp_OnSuccessCallback successCallback,
    m2tp_OnErrorCallback errorCallback)
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return;

  TransmitBuffer_finishAsync(successCallback, errorCallback);
}

void m2tp_createTopic(
    char *topicName,
    m2tp_OnRegisteredCallback successCallback,
    m2tp_OnErrorCallback failedCallback)
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return;

  TaskRouter_assignTopic(topicName, successCallback, NULL, failedCallback);
}

void m2tp_subscribe(
    char *topicName,
    m2tp_TopicListener listener)
{
  // Abort if device NOT ready yet
  if (!DeviceState_isReady())
    return;

  TaskRouter_assignTopic(topicName, NULL, listener, NULL);
}

m2tp_channel m2tp_getAddress()
{
  return DeviceState_assignedAddress;
}

m2tp_byte m2tp_isConnected()
{
  return DeviceState_isReady();
}

const char *m2tp_getDeviceBrand()
{
  return DeviceState_deviceBrand;
}

const char *m2tp_getDeviceVendor()
{
  return DeviceState_deviceVendor;
}

const char *m2tp_getDeviceClass()
{
  return DeviceState_deviceClass;
}

////////////////////////////////////////////////////////
