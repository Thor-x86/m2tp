/**
 * @file functions.h
 * @brief All functions here are shared among threads
**/

#ifndef IS_M2TP_VIRTUAL_GLUE_FUNCTIONS_DEFINED
#define IS_M2TP_VIRTUAL_GLUE_FUNCTIONS_DEFINED

/**
 * @brief Use this before modifying global variables
 *        from a thread other than main thread
 */
extern void pauseMainThread();

/**
 * @brief ALWAYS call this after pauseMainThread()
 */
extern void resumeMainThread();

#endif // #ifndef IS_M2TP_VIRTUAL_GLUE_FUNCTIONS_DEFINED
