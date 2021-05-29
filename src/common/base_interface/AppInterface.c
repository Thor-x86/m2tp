/**
 * @file AppInterface.c
 * @brief Implementation of "m2tp-common/base_interface/AppInterface.h"
**/

#include "m2tp-common/base_interface/AppInterface.h"
#include "../DeviceState.h"

//////// Variables /////////////////////////////////////

void (*m2tp_receivedListener)(
    m2tp_channel sourceAddress,
    unsigned short int dataSize,
    m2tp_bytes data) = NULL;

void (*m2tp_onNewMemberListener)(
    char *memberClass,
    m2tp_channel assignedAddress) = NULL;

void (*m2tp_onAnotherMemberQuitListener)(
    m2tp_channel assignedAddress) = NULL;

////////////////////////////////////////////////////////

//////// Functions /////////////////////////////////////

void m2tp_send(
    m2tp_channel targetAddress,
    unsigned short int dataSize,
    m2tp_bytes data)
{
  // TODO: Write stuffs here...
}

void m2tp_sendWithCallback(
    m2tp_channel targetAddress,
    unsigned short int dataSize,
    m2tp_bytes data,
    m2tp_OnSuccessCallback successCallback,
    m2tp_OnErrorCallback errorCallback)
{
  // TODO: Write stuffs here...
}

void m2tp_createTopic(
    char *topicName,
    m2tp_OnRegisteredCallback successCallback,
    m2tp_OnErrorCallback failedCallback)
{
  // TODO: Write stuffs here...
}

void m2tp_publish(m2tp_channel topicID, unsigned short int dataSize, m2tp_bytes data)
{
  // TODO: Write stuffs here...
}

void m2tp_publishWithCallback(
    m2tp_channel topicID,
    unsigned short int dataSize,
    m2tp_bytes data,
    m2tp_OnSuccessCallback successCallback,
    m2tp_OnErrorCallback errorCallback)
{
  // TODO: Write stuffs here...
}

void m2tp_subscribe(
    char *topicName,
    m2tp_TopicListener listener)
{
  // TODO: Write stuffs here...
}

m2tp_channel m2tp_getAddress()
{
  return assignedAddress;
}

m2tp_byte m2tp_isConnected()
{
  return isReady();
}

char *m2tp_getDeviceBrand()
{
  return deviceBrand;
}

char *m2tp_getDeviceVendor()
{
  return deviceVendor;
}

char *m2tp_getDeviceClass()
{
  return deviceClass;
}

////////////////////////////////////////////////////////
