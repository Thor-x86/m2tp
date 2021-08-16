#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include <m2tp-leader.h> // M2TP Library for Leader

// Green-colored "[Leader] " string
const std::string debugtag = "\e[1;32m[Leader]\e[0m ";

volatile m2tp_channel member1_address = 0;
volatile m2tp_channel member2_address = 0;
volatile m2tp_channel member3_address = 0;

volatile unsigned int count = 0;

//////////////// Function Header ////////////////////////////////////////////////

void onReceive(m2tp_channel sourceAddress, m2tp_byte dataSize, m2tp_bytes data);
void onNewMember(char *memberClass, m2tp_channel assignedAddress);
void onMemberQuit(m2tp_channel assignedAddress);

/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
  // Check for executable arguments
  if (argc < 2)
  {
    std::cerr << debugtag << "Missing second arg (port number)!" << std::endl;
    return EXIT_FAILURE;
  }

  // Resolve UDP port number from executable arguments
  unsigned short portNumber = atoi(argv[1]);

  // Configure M2TP then connect
  std::cout << debugtag << "Started, connecting to " << argv[1] << std::endl;
  m2tp_receivedListener = &onReceive;
  m2tp_onNewMemberListener = &onNewMember;
  m2tp_onAnotherMemberQuitListener = &onMemberQuit;
  bool connectSuccess = m2tp_startUDPServer("127.0.0.1", portNumber, "leader");

  // Connection failure mostly caused by UDP socket
  if (!connectSuccess)
  {
    std::cerr << debugtag
              << "Cannot communicate with Virtual Network"
              << std::endl;
    return EXIT_FAILURE;
  }

  // Waiting for members, until 5 seconds
  for (uint8_t i = 0; i < 10; i++)
  {
    if (member1_address > 0)
      break;
    sleep(1);
  }

  // Check for members presence
  if (member1_address == 0)
  {
    std::cerr << debugtag << "member #1 not connected!" << std::endl;
    m2tp_disconnect();
    return EXIT_FAILURE;
  }

  std::cout << debugtag << "Ready!" << std::endl;

  while (member1_address > 0)
  {
    if (count < 10)
      count++;
    m2tp_startSend((m2tp_channel)member1_address);
    std::string data = "Marco (" + std::to_string(count) + ")";
    for (std::size_t i = 0; i < data.size(); i++)
      m2tp_write(data[i]);
    m2tp_writeFinish();
    sleep(1);
  }

  if (count == 10)
  {
    std::cout << debugtag << "Finish!" << std::endl;
    return EXIT_SUCCESS;
  }
  else
  {
    std::cout << debugtag << "Expected 10 but got " << count << std::endl;
    return EXIT_FAILURE;
  }
}

void onReceive(m2tp_channel sourceAddress, m2tp_byte dataSize, m2tp_bytes data)
{
  // Convert raw bytes to proper string (ended with NUL character)
  char message[dataSize + 1];
  std::memcpy(message, data, dataSize);
  message[dataSize] = '\0';

  // Send to console
  if (sourceAddress == member1_address)
    std::cout << debugtag << "   Received data: " << message << std::endl;
  else if (sourceAddress == member2_address)
    std::cout << debugtag << "Received data from Member #2: " << message << std::endl;
  else if (sourceAddress == member3_address)
    std::cout << debugtag << "Received data from Member #3: " << message << std::endl;
  else
    std::cout << debugtag << "Received data from unknown (" << (unsigned int)sourceAddress << "): " << message << std::endl;
}

void onNewMember(char *memberClass, m2tp_channel assignedAddress)
{
  std::cout << debugtag
            << "\""
            << memberClass
            << "\" device connected with address "
            << std::to_string(assignedAddress)
            << std::endl;

  if (std::strcmp(memberClass, "member_1") == 0)
    member1_address = assignedAddress;
  else if (std::strcmp(memberClass, "member_2") == 0)
    member2_address = assignedAddress;
  else if (std::strcmp(memberClass, "member_3") == 0)
    member3_address = assignedAddress;
}

void onMemberQuit(m2tp_channel assignedAddress)
{
  std::cout << debugtag
            << std::to_string(assignedAddress)
            << " left the network"
            << std::endl;

  if (assignedAddress == member1_address)
    member1_address = 0;
  else if (assignedAddress == member2_address)
    member2_address = 0;
  else if (assignedAddress == member3_address)
    member3_address = 0;
}
