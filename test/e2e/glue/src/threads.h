/**
 * @file threads.h
 * @brief Binds all threads, so they can be accessed by main.c
**/

#ifndef IS_M2TP_VIRTUAL_GLUE_THREADS_DEFINED
#define IS_M2TP_VIRTUAL_GLUE_THREADS_DEFINED

/**
 * @brief Handles thread-blocking recvfrom(...) function
 * @return NULL
 */
extern void *ReceiverThread(void *);

/**
 * @brief Handles thread-blocking usleep(...) function
 * @return NULL
 */
extern void *TimeoutThread(void *);

#endif // #ifndef IS_M2TP_VIRTUAL_GLUE_THREADS_DEFINED
