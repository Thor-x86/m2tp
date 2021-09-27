/**
 * @file app.h
 * @brief Used by upper layer like m2tp-bson, m2tp-json, and more
**/

#ifndef IS_M2TP_INTERFACE_APP_DEFINED
#define IS_M2TP_INTERFACE_APP_DEFINED

#include "../typedef.h"
#include "../cpp_compat.h"

EXTERN_C_START

/**
 * @brief Universal callback, will be called when something success
 */
typedef void (*m2tp_OnSuccessCallback)();

/**
 * @brief Universal callback, will be called when something goes wrong
 * @param errorCode See "m2tp-common/errors.h" for details
 */
typedef void (*m2tp_OnErrorCallback)(m2tp_byte errorCode);

/**
 * @brief Will be called to give you the result of registration
 * @param assignedAs Keep this assigned identity
 */
typedef void (*m2tp_OnRegisteredCallback)(m2tp_channel assignedAs);

/**
 * @brief Will be called everytime data published at that topic from other device
 * @param dataSize In bytes, at most 253 bytes
 * @param data Retreived message data
 */
typedef void (*m2tp_TopicListener)(m2tp_byte dataSize, const m2tp_bytes data);

/**
 * @brief Prepare to send data in peer-to-peer mode
 * @param targetAddress Audience device's address
 * @return Error code, 0 means success
 */
extern m2tp_error m2tp_startSend(m2tp_channel targetAddress);

/**
 * @brief Prepare to send data in broadcast mode
 * @param topicID From success callback of `m2tp_createTopic(...)`
 * @return Error code, 0 means success
 */
extern m2tp_error m2tp_startBroadcast(m2tp_channel topicID);

/**
 * @brief When finished, call m2tp_writeFinish() or m2tp_writeFinishAsync(...)
 * @param eachByte from data
 * @return Error code, 0 means success
 */
extern m2tp_error m2tp_write(m2tp_byte eachByte);

/**
 * @brief Send written bytes of data
 * @return Error code, 0 means success
 */
extern m2tp_error m2tp_writeFinish();

/**
 * @brief Use this to prevent thread-blocking because of repetitive send/broadcast
 * @param successCallback Pointer to the next function
 * @param errorCallback Pointer to a function, has a byte paramater represents error code
 */
extern void m2tp_writeFinishAsync(m2tp_OnSuccessCallback successCallback, m2tp_OnErrorCallback errorCallback);

/**
 * @brief Pointer to a function, will be called when receiving something in peer-to-peer
 * @param sourceAddress Identity of the sender
 * @param dataSize In bytes, at most 253 bytes
 * @param data Received raw bytes of data
 */
extern void (*m2tp_receivedListener)(m2tp_channel sourceAddress, m2tp_byte dataSize, const m2tp_bytes data);

/**
 * @brief Creates a topic for the network, use this before publish
 * @param topicName An identity that meant to be shared among other programmers
 * @param successCallback Pointer to a function, you will get topic ID from here
 * @param failedCallback Pointer to a function, will be called on failed
 */
extern void m2tp_createTopic(char *topicName, m2tp_OnRegisteredCallback successCallback, m2tp_OnErrorCallback failedCallback);

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
 * @brief Pointer to a function, will be called when another member left the network
 * @param assignedAddress That member's address
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
extern const char *m2tp_getDeviceBrand();

/**
 * @brief Get current device manufacturer, from driver interface
 * @return char* (Primitive String)
 */
extern const char *m2tp_getDeviceVendor();

/**
 * @brief Get current device class, from driver interface
 * @return char* (Primitive String)
 */
extern const char *m2tp_getDeviceClass();

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_INTERFACE_APP_DEFINED
