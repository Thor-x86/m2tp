/**
 * @file m2tp-virtual.h
 * @brief Connects M2TP on virtual devices to virtual network
**/

#ifndef IS_M2TP_VIRTUAL_GLUE_DEFINED
#define IS_M2TP_VIRTUAL_GLUE_DEFINED

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief Give M2TP instruction to connect to virtual network
   * @param port TCP port of virtual network
   * @param deviceClass tells the role of current device
   * @return true if success, otherwise failed
   */
  extern bool m2tp_connect(unsigned short port, char *deviceClass);

  /**
   * @brief Give M2TP instruction to disconnect from virtual network
   */
  extern void m2tp_disconnect();

#ifdef __cplusplus
}
#endif

#endif // #ifndef IS_M2TP_VIRTUAL_GLUE_DEFINED
