/**
 * @file DriverInterface.h
 * @brief Basic functionality of Driver Interface
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
 * @brief Ask M2TP to prepare packet buffer
 * @param command See "m2tp-common/commands.h"
 * @param contentSize Size of packet content, maximum 255 bytes
 */
extern void m2tp_driver_receiveStart(m2tp_byte command, m2tp_byte contentSize);

/**
 * @brief Put each byte to packet buffer
 * @param value a Single byte of packet content
 */
extern void m2tp_driver_receiveWrite(m2tp_byte value);

/**
 * @brief Ask M2TP to mark it done and do something with that packet
 */
extern void m2tp_driver_receiveEnd();

/**
 * @brief Report if driver cannot receive packet correctly
 * @param errorCode See "m2tp-common/errors.h"
 */
extern void m2tp_driver_receiveFailed(m2tp_byte errorCode);

/**
 * @brief Pointer to a function, will be called if app need to send something
 * @param command See "m2tp-common/commands.h"
 * @param contentSize In bytes
 * @param content Serialized packet content
 */
extern void (*m2tp_driver_sendListener)(m2tp_byte command, m2tp_byte contentSize, const m2tp_bytes content);

/**
 * @brief Report if driver cannot send packet
 * @param errorCode See "m2tp-common/errors.h"
 */
extern void m2tp_driver_sendFailed(m2tp_byte errorCode);

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
