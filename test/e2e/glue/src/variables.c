#include "variables.h"

int socketDescriptor = 0;

struct sockaddr_in vnAddress;
socklen_t addressSize = sizeof(struct sockaddr_in);

pthread_t mainThreadID = 0;
pthread_t receiverThreadID = 0;
pthread_t timeoutThreadID = 0;

sem_t mainThreadUnblocker;
pthread_mutex_t mainThreadUnblockerMutex = PTHREAD_MUTEX_INITIALIZER;

volatile struct Timeout timeout = {0UL, NULL};
pthread_mutex_t timeoutMutex = PTHREAD_MUTEX_INITIALIZER;
