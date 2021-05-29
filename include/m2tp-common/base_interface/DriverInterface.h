/**
 * @file DriverInterface.h
 * @brief Basic functionality of Driver Interface, written in plain C
**/

#ifndef IS_M2TP_COMMON_DRIVER_INTERFACE_DEFINED
#define IS_M2TP_COMMON_DRIVER_INTERFACE_DEFINED

#include "../typedef.h"

/**
 * @brief Will be called after timer finish
 */
typedef void (*m2tp_driver_TimerFinishCallback)();

/**
 * @brief Set device's brand, it's optional
 */
extern void m2tp_driver_setDeviceBrand(char *value);

/**
 * @brief Set device's manufacturer, it's optional
 */
extern void m2tp_driver_setDeviceVendor(char *value);

/**
 * @brief Set the device's role, required to inform other device
 */
extern void m2tp_driver_setDeviceClass(char *value);

/**
 * @brief Notify library if connected from network
 */
extern void m2tp_driver_connected();

/**
 * @brief Notify library if disconnected from network
 */
extern void m2tp_driver_disconnected();

/**
 * @brief Notify library if receive something from network
 * @param command See commands.h for details
 * @param contentSize In bytes
 * @param content Serialized packet content
 */
extern void m2tp_driver_receive(m2tp_byte command, unsigned short int contentSize, m2tp_bytes content);

/**
 * @brief Report if driver cannot receive packet correctly
 * @param errorCode See errors.hpp for details
 * @param where Address / Topic causes error
 */
extern void m2tp_driver_receiveFailed(m2tp_byte errorCode, m2tp_channel where);

/**
 * @brief Pointer to a function, will be called if app need to send something
 * @param command See commands.h for details
 * @param contentSize In bytes
 * @param content Serialized packet content
 */
extern void (*m2tp_driver_sendListener)(m2tp_byte command, unsigned short int contentSize, m2tp_bytes content);

/**
 * @brief Report if driver cannot send packet
 * @param errorCode See errors.hpp for details
 * @param where Address / TopicID causes error
 */
extern void m2tp_driver_sendFailed(m2tp_byte errorCode, m2tp_channel where);

/**
 * @brief Pointer to a function, driver decides how delay behaves
 * @param duration In milliseconds
 * @param callback Provided by M2TP Library, must be called after wait is done
 */
extern void (*m2tp_driver_startTimerListener)(unsigned long int duration, m2tp_driver_TimerFinishCallback callback);

/**
 * @brief Pointer to a function, driver decides how delay behaves
 */
extern void (*m2tp_driver_stopTimerListener)();

#endif // #ifndef IS_M2TP_COMMON_DRIVER_INTERFACE_DEFINED
