/**
 * @file m2tp-posix.h
 * @brief For UNIX-like machines, automatically included
 * @note Already included in m2tp-leader and m2tp-leader, you don't have to include this again
**/

#ifndef IS_M2TP_POSIX_DEFINED
#define IS_M2TP_POSIX_DEFINED

#include "./m2tp/interface/driver.h"

EXTERN_C_START

/**
 * @brief This is how receive hook declared
 * @param input Serialized frame from network interface
 * @param inputSize In bytes
 * @param output Serialized packet for M2TP
 * @returns Size of output, in bytes
 */
typedef m2tp_byte (*m2tp_posix_ReceiveHook)(m2tp_bytes input, m2tp_byte inputSize, m2tp_bytes output);

/**
 * @brief This is how transmit hook declared
 * @param input Serialized packet from M2TP
 * @param inputSize In bytes
 * @param output Serialized frame for network interface
 * @returns Size of output, in bytes
 */
typedef m2tp_byte (*m2tp_posix_TransmitHook)(m2tp_bytes input, m2tp_byte inputSize, m2tp_bytes output);

/**
 * @brief M2TP-POSIX uses SIGUSR1 by default, call this before connect to change
 * @param signalCode See https://dsa.cs.tsinghua.edu.cn/oj/static/unix_signal.html
 */
extern void m2tp_useSignal(int signalCode);

/**
 * @brief Allows accomodate lower OSI layer frame structure, only call this before connect
 * @warning The hooks will reset after calling m2tp_disconnect()
 * @param frameSize Maximum network's frame size, in bytes
 * @param receive Pointer to a function, from network interface to M2TP
 * @param transmit Pointer to a function, from M2TP to network interface
 */
extern void m2tp_useHook(unsigned long frameSize, m2tp_posix_ReceiveHook receive, m2tp_posix_TransmitHook transmit);

/**
 * @brief Use this if you experiences data loss
 * @param packetSize In bytes
 */
extern void m2tp_limitPacketSize(unsigned long packetSize);

/**
 * @brief Connect to M2TP network via device file
 * @param filePath Path to the device file
 * @param deviceClass Role of current device
 * @return true if success
 */
extern bool m2tp_connectViaFile(const char *filePath, const char *deviceClass);

/**
 * @brief Connect to M2TP network via pre-configured file descriptor
 * @note Only for experienced programmers, please use `m2tp_connectViaFile` instead
 * @param fileDescriptor From return value of open()
 * @param deviceClass Role of current device
 * @return true if success
 */
extern bool m2tp_connectViaPreconfigFile(int fileDescriptor, const char *deviceClass);

/**
 * @brief Connect to M2TP network via opened socket by kernel
 * @param socketDescriptor From return value of connect()
 * @param deviceClass Role of current device
 * @return true if success
 */
extern bool m2tp_connectViaSocket(int socketDescriptor, const char *deviceClass);

/**
 * @brief Connect to M2TP network via SocketCAN
 * @param interfaceName As example: "can0", "can1", "vcan0", ...
 * @param canID Fill this with 0, unless coexist with protocol-less device is required
 * @param deviceClass Role of current device
 * @return true if success
 */
extern bool m2tp_connectViaCAN(const char *interfaceName, unsigned int canID, const char *deviceClass);

// Prevent member devices use m2tp_startUDPServer
#ifndef IS_M2TP_MEMBER_DEFINED

/**
 * @brief Start a new network via UDP socket
 * @warning Only for leader device
 * @param ipAddress String of leader device's IPv4 address
 * @param port Leader device's UDP port number
 * @param deviceClass Role of current device
 * @return true if success
 */
extern bool m2tp_startUDPServer(const char *ipAddress, unsigned short port, const char *deviceClass);

#endif // #ifndef IS_M2TP_MEMBER_DEFINED

// Prevent leader device use m2tp_connectViaUDP
#ifndef IS_M2TP_LEADER_DEFINED

/**
 * @brief Simplify code for connecting via UDP
 * @warning Leader must use m2tp_startUDPServer instead
 * @param ipAddress String of current device's IPv4 address
 * @param port Set 0 to generate port number automatically
 * @param leaderIPAddress String of leader device's IPv4 address
 * @param leaderPort Leader device's UDP port number
 * @param deviceClass Role of current device
 * @return true if success
 */
extern bool m2tp_connectViaUDP(const char *ipAddress, unsigned short port, const char *leaderIPAddress, unsigned short leaderPort, const char *deviceClass);

#endif // #ifndef IS_M2TP_LEADER_DEFINED

/**
 * @brief Disconnect from file/socket and M2TP side
 */
extern void m2tp_disconnect();

EXTERN_C_STOP

#endif // #ifndef IS_M2TP_POSIX_DEFINED
