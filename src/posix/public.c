/**
 * @file public.c
 * @brief Implementation of "include/m2tp-posix.h"
**/

#include "m2tp-posix.h"
#include "./dependencies.h"
#include "./variables.h"
#include "./thread/MainThread.h"
#include "./UdpServer.h"
#include "./CanHook.h"

void m2tp_useSignal(int signalCode)
{
  // Only change signal code while offline,
  // otherwise undefined behavior occurs
  if (connectMode == MODE_OFFLINE)
    assignedSignalCode = signalCode;
}

void m2tp_useHook(
    unsigned long frameSize,
    m2tp_posix_ReceiveHook receive,
    m2tp_posix_TransmitHook transmit)
{
  // Only set hooks while offline,
  // this also prevents undefined behavior
  if (connectMode == MODE_OFFLINE)
  {
    maxFrameSize = frameSize;
    receiveHook = receive;
    transmitHook = transmit;
  }
}

bool m2tp_connectViaFile(
    const char *filePath,
    const char *deviceClass)
{
  int fileDescriptor = open(filePath, O_RDWR);
  return m2tp_connectViaPreconfigFile(fileDescriptor, deviceClass);
}

bool m2tp_connectViaPreconfigFile(
    int fileDescriptor,
    const char *deviceClass)
{
  // Check for file descriptor
  if (fileDescriptor <= 0)
    return false;

  // Making sure not connected yet
  if (connectMode != MODE_OFFLINE)
    return true;

  descriptor = fileDescriptor;
  connectMode = MODE_FILE;
  MainThread_attach(deviceClass);
}

bool m2tp_connectViaSocket(
    int socketDescriptor,
    const char *deviceClass)
{
  // Check for socket descriptor
  if (socketDescriptor <= 0)
    return false;

  // Making sure not connected yet
  if (connectMode != MODE_OFFLINE)
    return true;

  descriptor = socketDescriptor;
  connectMode = MODE_SOCKET;
  MainThread_attach(deviceClass);
}

bool m2tp_connectViaCAN(
    const char *interfaceName,
    unsigned int canID,
    const char *deviceClass)
{
  // Making sure not connected yet
  if (connectMode != MODE_OFFLINE)
    return true;

  // Reusable return code variable
  int returnCode = 0;

  // Request socket descriptor to kernel
  int socketDescriptor = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (socketDescriptor < 0)
    return false;

  // Attach to SocketCAN, it will return CAN ID
  struct ifreq interfaceRequest;
  memset(&interfaceRequest, 0, sizeof(struct ifreq));
  strcpy(interfaceRequest.ifr_name, interfaceName);
  ioctl(socketDescriptor, SIOCGIFINDEX, &interfaceRequest);

  // Wrap returned CAN ID with sockaddr_can
  struct sockaddr_can myAddress;
  memset(&myAddress, 0, sizeof(struct sockaddr_can));
  myAddress.can_family = AF_CAN;
  myAddress.can_ifindex = interfaceRequest.ifr_ifindex;

  // Prepare receive (Rx)
  returnCode = bind(socketDescriptor,
                    (struct sockaddr *)&myAddress,
                    sizeof(struct sockaddr_can));
  if (returnCode < 0)
    return false;

  // Assign the CAN ID
  CanHook_ID = canID;

  // Attach hooks to process CAN frame
  m2tp_useHook(sizeof(struct can_frame), &CanHook_receive, &CanHook_transmit);

  descriptor = socketDescriptor;
  connectMode = MODE_SOCKET;
  MainThread_attach(deviceClass);
}

bool m2tp_startUDPServer(
    const char *ipAddress,
    unsigned short port,
    const char *deviceClass)
{
  // Making sure not connected yet
  if (connectMode != MODE_OFFLINE)
    return true;

  // Reusable return code variable
  int returnCode = 0;

  // Assemble UDP+IPv4 information
  struct sockaddr_in myAddress;
  memset(&myAddress, 0, sizeof(struct sockaddr_in));
  myAddress.sin_family = AF_INET;
  myAddress.sin_port = htons(port);
  returnCode = inet_pton(AF_INET, ipAddress, &(myAddress.sin_addr));
  if (returnCode != 1)
    return false;

  // Create temporary variable for socket descriptor
  int socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socketDescriptor < 0)
    return false;

  // Prepare receive (Rx)
  returnCode = bind(socketDescriptor,
                    (struct sockaddr *)&myAddress,
                    sizeof(struct sockaddr_in));
  if (returnCode < 0)
    return false;

  // Start the UDP Server as transmitter (Tx)
  UdpServer_enabled = true;
  UdpServer_clear();

  // Start all threads and init the M2TP
  descriptor = socketDescriptor;
  connectMode = MODE_SOCKET;
  MainThread_attach(deviceClass);
  return true;
}

bool m2tp_connectViaUDP(
    const char *ipAddress,
    unsigned short port,
    const char *leaderIPAddress,
    unsigned short leaderPort,
    const char *deviceClass)
{
  // Making sure not connected yet
  if (connectMode != MODE_OFFLINE)
    return true;

  // Reusable return code variable
  int returnCode = 0;

  // Assemble current device's UDP+IPv4 information
  struct sockaddr_in myAddress;
  memset(&myAddress, 0, sizeof(struct sockaddr_in));
  myAddress.sin_family = AF_INET;
  myAddress.sin_port = htons(port);
  returnCode = inet_pton(AF_INET, ipAddress, &(myAddress.sin_addr));
  if (returnCode != 1)
    return false;

  // Assemble leader's UDP+IPv4 information
  struct sockaddr_in leaderAddress;
  memset(&leaderAddress, 0, sizeof(struct sockaddr_in));
  leaderAddress.sin_family = AF_INET;
  leaderAddress.sin_port = htons(leaderPort);
  returnCode = inet_pton(AF_INET, leaderIPAddress, &(leaderAddress.sin_addr));
  if (returnCode != 1)
    return false;

  // Create temporary variable for socket descriptor
  int socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socketDescriptor < 0)
    return false;

  // Prepare receive (Rx)
  returnCode = bind(socketDescriptor,
                    (struct sockaddr *)&myAddress,
                    sizeof(struct sockaddr_in));
  if (returnCode < 0)
    return false;

  // Prepare transmit (Tx)
  returnCode = connect(socketDescriptor,
                       (struct sockaddr *)&leaderAddress,
                       sizeof(struct sockaddr_in));
  if (returnCode < 0)
    return false;

  // Transmit for the first time
  returnCode = write(socketDescriptor, NULL, 0);
  if (returnCode < 0)
    return false;

  // Start all threads and init the M2TP
  descriptor = socketDescriptor;
  connectMode = MODE_SOCKET;
  MainThread_attach(deviceClass);
  return true;
}

void m2tp_disconnect()
{
  // Making sure currently connected
  if (connectMode == MODE_OFFLINE)
    return;

  // Making sure UDP Server disabled
  UdpServer_enabled = false;

  // Stop M2TP and all threads
  MainThread_detach();

  // Close file/socket gracefully
  if (connectMode == MODE_SOCKET)
    shutdown(descriptor, SHUT_RDWR);
  close(descriptor);

  // Reset the hook
  receiveHook = NULL;
  transmitHook = NULL;
  maxFrameSize = 0;
  CanHook_ID = 0;

  connectMode = MODE_OFFLINE;
}
