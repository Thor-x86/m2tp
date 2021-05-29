/**
 * @file AppInterface.h
 * @brief Basic functionality of App Interface
**/

#ifndef IS_M2TP_COMMON_APP_INTERFACE_DEFINED
#define IS_M2TP_COMMON_APP_INTERFACE_DEFINED

#include "../typedef.h"

/**
 * @brief Universal callback, will be called when something success
 */
typedef void (*m2tp_OnSuccessCallback)();

/**
 * @brief Universal callback, will be called when something goes wrong
 * @param errorCode See `m2tp-common/errors.hpp` for details
 */
typedef void (*m2tp_OnErrorCallback)(m2tp_byte errorCode);

/**
 * @brief Will be called to give you the result of registration
 * @param assignedAs Keep this assigned identity
 */
typedef void (*m2tp_OnRegisteredCallback)(m2tp_channel assignedAs);

/**
 * @brief Will be called everytime data published at that topic from other device
 * @param dataSize In bytes
 * @param data Retreived message data
 */
typedef void (*m2tp_TopicListener)(unsigned short int dataSize, m2tp_bytes data);

/**
 * @brief Send data to a device, in peer-to-peer mode
 * @param targetAddress Intended device
 * @param dataSize Unit in bytes
 * @param data Raw bytes of data to be sent
 */
extern void m2tp_send(m2tp_channel targetAddress, unsigned short int dataSize, m2tp_bytes data);

/**
 * @brief Send data to a device, in peer-to-peer mode
 * @param targetAddress Intended device
 * @param dataSize Unit in bytes
 * @param data Raw bytes of data to be sent
 * @param successCallback Pointer to a function, will be called on success
 * @param failedCallback Pointer to a function, will be called on failed
 */
extern void m2tp_sendWithCallback(m2tp_channel targetAddress, unsigned short int dataSize, m2tp_bytes data, m2tp_OnSuccessCallback successCallback, m2tp_OnErrorCallback errorCallback);

/**
 * @brief Pointer to a function, will be called when receiving something in peer-to-peer
 * @param sourceAddress Identity of the sender
 * @param dataSize In bytes
 * @param data Received raw bytes of data
 */
extern void (*m2tp_receivedListener)(m2tp_channel sourceAddress, unsigned short int dataSize, m2tp_bytes data);

/**
 * @brief Creates a topic for the network, use this before publish
 * @param topicName An identity that meant to be shared among other programmers
 * @param successCallback Pointer to a function, you will get topic ID from here
 * @param failedCallback Pointer to a function, will be called on failed
 */
extern void m2tp_createTopic(char *topicName, m2tp_OnRegisteredCallback successCallback, m2tp_OnErrorCallback failedCallback);

/**
 * @brief Broadcast data to a topic
 * @param topicID You got this from result of `createTopic(...)` function
 * @param dataSize Unit in bytes
 * @param data Raw bytes of data to be broadcasted
 */
extern void m2tp_publish(m2tp_channel topicID, unsigned short int dataSize, m2tp_bytes data);

/**
 * @brief Broadcast data to a topic
 * @param topicID You got this from result of `createTopic(...)` function
 * @param dataSize Unit in bytes
 * @param data Raw bytes of data to be broadcasted
 * @param successCallback Pointer to a function, will be called on success
 * @param failedCallback Pointer to a function, will be called on failed
 */
extern void m2tp_publishWithCallback(m2tp_channel topicID, unsigned short int dataSize, m2tp_bytes data, m2tp_OnSuccessCallback successCallback, m2tp_OnErrorCallback errorCallback);

/**
 * @brief Listen to data coming from a topic
 * @param topicName An identity that meant to be shared among other programmers
 * @param listener Will be called everytime received data
 */
extern void m2tp_subscribe(char *topicName, m2tp_TopicListener listener);

/**
 * @brief Pointer to a function, will be called when new member registered
 * @param memberClass Type of device, came from that device's driver interface
 * @param assignedAddress New member's address
 */
extern void (*m2tp_onNewMemberListener)(char *memberClass, m2tp_channel assignedAddress);

/**
 * @brief Pointer to a function, will be called when new member registered
 * @param memberClass Type of device, came from that device's driver interface
 * @param assignedAddress New member's address
 */
extern void (*m2tp_onAnotherMemberQuitListener)(m2tp_channel assignedAddress);

/**
 * @brief Get currently assigned address
 * @return Address, but nullptr if not assigned yet
 */
extern m2tp_channel m2tp_getAddress();

/**
 * @brief Check if currently connected or not
 * @return 1 means connected
 * @return 0 means disconnected
 */
extern m2tp_byte m2tp_isConnected();

/**
 * @brief Get current device name, from driver interface
 * @return char* (Primitive String)
 */
extern char *m2tp_getDeviceBrand();

/**
 * @brief Get current device manufacturer, from driver interface
 * @return char* (Primitive String)
 */
extern char *m2tp_getDeviceVendor();

/**
 * @brief Get current device class, from driver interface
 * @return char* (Primitive String)
 */
extern char *m2tp_getDeviceClass();

#endif // #ifndef IS_M2TP_COMMON_APP_INTERFACE_DEFINED
