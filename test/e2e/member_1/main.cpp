#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include <m2tp-member/AppInterface.h> // M2TP Library for Leader
#include <m2tp-virtual.h>             // Glue between M2TP and Virtual Network

// Blue-colored "[Member #1] " string
const std::string debugtag = "\e[1;34m[Member #1]\e[0m ";

m2tp_channel deviceAddress = 0;

volatile unsigned int count = 0;

//////////////// Function Header ////////////////////////////////////////////////

void onConnected(m2tp_channel address);
void onReceive(m2tp_channel sourceAddress, m2tp_byte dataSize, m2tp_bytes data);
void onDisconnected();

/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
  // Check for executable arguments
  if (argc < 2)
  {
    std::cerr << debugtag << "Missing second arg (port number)!" << std::endl;
    return 1;
  }

  // Resolve UDP port number from executable arguments
  unsigned short portNumber = atoi(argv[1]);

  // Configure M2TP then connect
  std::cout << debugtag << "Started, connecting to " << argv[1] << std::endl;
  m2tp_receivedListener = &onReceive;
  m2tp_onConnected = &onConnected;
  m2tp_onDisconnected = &onDisconnected;
  bool connectSuccess = m2tp_connect(portNumber, "member_1");

  // Connection failure mostly caused by UDP socket
  if (!connectSuccess)
  {
    std::cerr << debugtag
              << "Cannot communicate with Virtual Network"
              << std::endl;
    return EXIT_FAILURE;
  }

  // Wait for connection, until 5 seconds
  for (uint8_t i = 0; i < 5; i++)
  {
    if (deviceAddress > 0)
      break;
    sleep(1);
  }

  // Check for connection
  if (deviceAddress == 0)
  {
    std::cerr << debugtag << "Connection failed!" << std::endl;
    return EXIT_FAILURE;
  }

  // Now, let onReceive(...) handles the rest
  while (deviceAddress > 0)
    sleep(10);

  // Below supposed to be non reachable
  std::cerr << debugtag << "Oops, loop broken" << std::endl;
  return EXIT_FAILURE;
}

void onConnected(m2tp_channel address)
{
  deviceAddress = address;
  std::cout << debugtag
            << "Ready! device address: "
            << (unsigned int)address
            << std::endl;
}

void onReceive(m2tp_channel sourceAddress, m2tp_byte dataSize, m2tp_bytes data)
{
  // Convert raw bytes to proper string (ended with NUL character)
  char message[dataSize + 1];
  std::memcpy(message, data, dataSize);
  message[dataSize] = '\0';

  if (count < 10)
    count++;
  else
  {
    m2tp_disconnect();
    return;
  }

  // Send to console
  std::cout << debugtag
            << "Received data: "
            << message
            << std::endl;

  // Reply the message
  m2tp_startSend(sourceAddress);
  std::string sendMessage = "Polo (" + std::to_string(count) + ")";
  for (size_t i = 0; i < sendMessage.size(); i++)
    m2tp_write(sendMessage[i]);
  m2tp_writeFinish();
}

void onDisconnected()
{
  // Do cleanup
  deviceAddress = 0;

  if (count == 10)
  {
    std::cout << debugtag << "Success!" << std::endl;
    exit(EXIT_SUCCESS);
  }
  else
  {
    std::cerr << debugtag << "Connection aborted!" << std::endl;
    exit(EXIT_FAILURE);
  }
}
