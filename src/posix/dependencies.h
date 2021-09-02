/**
 * @file dependencies.h
 * @brief All shared includes should be here
**/

#ifndef IS_M2TP_POSIX_DEPENDENCIES_DEFINED
#define IS_M2TP_POSIX_DEPENDENCIES_DEFINED

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include "m2tp/interface/driver.h"

#endif // #ifndef IS_M2TP_POSIX_DEPENDENCIES_DEFINED
