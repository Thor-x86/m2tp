/**
 * @file driver.h
 * @brief Used by lower layer like m2tp-posix, m2tp-arduino-gpio, m2tp-arduino-mcp2515, and more
**/

#ifndef IS_M2TP_INTERFACE_DRIVER_DEFINED
#define IS_M2TP_INTERFACE_DRIVER_DEFINED

#include "../typedef.h"
#include "../cpp_compat.h"

EXTERN_C_START

/**
 * @brief Will be called after timer finish
 */
typedef void (*m2tp_driver_TimerFinishCallback)();

/**
 * @brief Set device's brand, it's optional
 */
extern void m2tp_driver_setDeviceBrand(const char *value);

/**
 * @brief Set device's manufacturer, it's optional
 */
extern void m2tp_driver_setDeviceVendor(const char *value);

/**
 * @brief Set the device's role, required to inform other device
 */
extern void m2tp_driver_setDeviceClass(const char *value);

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
 * @return true if it's safe to write the packet buffer now
 */
extern bool m2tp_driver_receiveStart(m2tp_byte command, m2tp_byte contentSize);

/**
 * @brief Put each byte to packet buffer
 * @param value a Single byte of packet content
 * @return Remaining bytes of content
 */
extern m2tp_byte m2tp_driver_receiveWrite(m2tp_byte value);

/**
 * @brief Always call this after each packet fragment received to prevent timeout
 */
extern void m2tp_driver_receiveFragmentEnd();

/**
 * @brief Ask M2TP to mark it done and do something with that packet
 * @return Error code, 0 means success
 */
extern m2tp_error m2tp_driver_receiveEnd();

/**
 * @brief Pointer to a function, will be called if app need to send something
 * @param command See "m2tp-common/commands.h"
 * @param contentSize In bytes
 * @param content Serialized packet content
 */
extern void (*m2tp_driver_sendListener)(m2tp_byte command, m2tp_byte contentSize, const m2tp_bytes content);

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

/**
 * @brief Pointer to a function, driver decides what to do while waiting packet being transmitted
 */
extern void (*m2tp_driver_onWaitForQueue)();

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_INTERFACE_DRIVER_DEFINED
