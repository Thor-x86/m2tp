/**
 * @file commands.h
 * @brief Available commands that you can use
**/

#ifndef IS_M2TP_COMMANDS_DEFINED
#define IS_M2TP_COMMANDS_DEFINED

#include "typedef.h"

/**
 * @brief Send by leader to force stop member's transmission
 */
#define M2TP_COMMAND_END_TRANSMISSION 0b10100000 | 0

/**
 * @brief Send by leader to announce a member just exited
 */
#define M2TP_COMMAND_ANNOUNCEMENT_QUIT 0b10100000 | 1

/**
 * @brief Send by leader to announce new member
 */
#define M2TP_COMMAND_ANNOUNCEMENT_JOIN 0b10100000 | 2

/**
 * @brief Send by leader to ask who are unregistered members
 */
#define M2TP_COMMAND_REGISTRATION_SIGNAL 0b10100000 | 3

/**
 * @brief Send by leader to allow member transmit data
 */
#define M2TP_COMMAND_CHANNEL_TURN_SIGNAL 0b10100000 | 4

/**
 * @brief Send by member to reply REGISTRATION_SIGNAL if not registered yet
 */
#define M2TP_COMMAND_REQUEST_REGISTER_DEVICE 0b10100000 | 5

/**
 * @brief Send by member to reply REGISTRATION_SIGNAL for broadcast
 */
#define M2TP_COMMAND_REQUEST_REGISTER_TOPIC 0b10100000 | 6

/**
 * @brief Send by leader to reply REQUEST_REGISTER_DEVICE with assigned address
 */
#define M2TP_COMMAND_RESPONSE_REGISTER_DEVICE 0b10100000 | 7

/**
 * @brief Send by leader to reply REQUEST_REGISTER_TOPIC with assigned topicID
 */
#define M2TP_COMMAND_RESPONSE_REGISTER_TOPIC 0b10100000 | 8

/**
 * @brief Send by leader to reintroduce existing member to a new member
 */
#define M2TP_COMMAND_REINTRODUCTION 0b10100000 | 9

/**
 * @brief Send by anyone for sending data
 */
#define M2TP_COMMAND_TRANSMIT 0b10100000 | 10

/**
 * @brief Send by anyone when something goes wrong
 */
#define M2TP_COMMAND_FAIL_SIGNAL 0b10100000 | 11

/**
 * @brief Send by anyone to sign that the data transmitted successfully
 */
#define M2TP_COMMAND_SUCCESS_SIGNAL 0b10100000 | 12

#endif // #ifndef IS_M2TP_COMMANDS_DEFINED
