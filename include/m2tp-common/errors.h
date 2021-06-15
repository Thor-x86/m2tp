/**
 * @file errors.h
 * @brief Standard error codes
**/

#ifndef IS_M2TP_ERRORS_DEFINED
#define IS_M2TP_ERRORS_DEFINED

/**
 * @brief Thrown when currently not connected to network
 */
#define M2TP_ERROR_NOT_CONNECTED 1

/**
 * @brief Intentional transmit failure
 */
#define M2TP_ERROR_TRANSMISSION_ABORT 2

/**
 * @brief Targeted address not exist, thrown from leader
 */
#define M2TP_ERROR_ADDRESS_NOT_EXIST 3

/**
 * @brief Targeted topic not exist, thrown from leader
 */
#define M2TP_ERROR_TOPIC_NOT_EXIST 4

/**
 * @brief Data loss happened, thrown from receiver to transmitter and receiver itself
 */
#define M2TP_ERROR_PACKET_SIZE_MISMATCH 5

/**
 * @brief When app gives incorrect data size
 */
#define M2TP_ERROR_DATA_SIZE_MISMATCH 6

/**
 * @brief CRC/MD5 failure, if driver supports CRC or MD5 check
 */
#define M2TP_ERROR_DATA_CORRUPT 7

/**
 * @brief Thrown if data size bigger than 64KB
 */
#define M2TP_ERROR_DATA_SIZE_TOO_BIG 8

/**
 * @brief One network can only holds 1 leader and 126 members
 */
#define M2TP_ERROR_TOO_MUCH_ADDRESS 9

/**
 * @brief One network can only holds 127 topics
 */
#define M2TP_ERROR_TOO_MUCH_TOPIC 10

#endif // #ifndef IS_M2TP_ERRORS_DEFINED
