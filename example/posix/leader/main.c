#include <stdlib.h>      // Core POSIX functionality
#include <stdio.h>       // Command line interface (CLI)
#include <string.h>      // Utility for handling multi-byte data
#include <m2tp-leader.h> // M2TP Library, as a leader

// NOTE: You don't have to include m2tp-posix.h because
//       it's already included in m2tp-leader.h

// Your unique device class,
// what is your device's role in the network?
static const char myClass[] = "Leader Device";

// Function declaration, these lines actually tells the compiler there are functions that
// will be defined somewhere at the bottom of this example code. So you don't have to put
// everything above main function, which is more convenient.
void onNewMember(char *memberClass, m2tp_channel assignedAddress);
void onReceiveData(m2tp_channel fromAddress, m2tp_byte dataSize, const m2tp_bytes data);
void onMemberQuit(m2tp_channel assignedAddress);

// Everytime a new member connected, its address will be stored here.
// So everytime you send message, you'll send that to this address.
volatile m2tp_channel targetAddress = 0;

int main()
{
  // Tell M2TP which function can be use
  m2tp_onNewMemberListener = &onNewMember;
  m2tp_receivedListener = &onReceive;
  m2tp_onAnotherMemberQuitListener = &onMemberQuit;

  // Example: connect via serial hardware (access via /dev/ttyS0)
  //bool success = m2tp_connectViaFile("/dev/ttyS0", myClass);

  // Example: create M2TP network with UDP port 32000
  bool success = m2tp_startUDPServer("127.0.0.1", 32000, myClass);

  // Example: connect via can0 interface
  //bool success = m2tp_connectViaCAN("can0", 0, myClass);

  // Show error and exit when failed to use intended network interface
  if (!success)
  {
    perror("Failed to engage with network interface");
    return EXIT_FAILURE;
  }

  // This is where your computer store your message
  // while you are typing
  char buffer[253];

  // ATTENTION: Usable payload size on M2TP is 253. Thus, you can
  //            allocate lower than that but cannot more than 253.

  // Program's main loop
  while (true)
  {
    // Clear the buffer, you have to do this at the beginning of the
    // loop because buffer initially filled with garbage letters
    memset(buffer, 0, sizeof(buffer));

    // Save everything you type to buffer and wait until you press enter
    fgets(buffer, sizeof(buffer), stdin);

    // Initiate send message to targeted address
    m2tp_startSend(targetAddress);

    // This loop stream message from your terminal to targeted device
    for (unsigned int i = 0; i < sizeof(buffer); i++)
    {
      char each = buffer[i];
      m2tp_write(each);

      // Remember, every string ended with NUL character.
      // So we can finish the stream when detect NUL character.
      if (each == '\0')
        break;
    }

    // Tell M2TP the message is finished
    m2tp_writeFinish();

    // Inform to command line that the message successfully sent
    printf("[SENT]\n");
  }

  // The program supposed to be endless loop (except hit CTRL+C)
  return EXIT_FAILURE;
}

// When a new member connected, M2TP will tell you via this function.
// You have to store its address to a variable if you want to send message to it.
void onNewMember(char *deviceClass, m2tp_channel address)
{
  // We will always send message to the latest member for this example
  targetAddress = address;

  // Inform a new member to command line
  printf("[INFO] %s connected as address #%d\n", deviceClass, address);
}

// When other device send message to this device, M2TP will tell you via this function
void onReceive(m2tp_channel fromAddress, m2tp_byte size, m2tp_bytes data)
{
  // Leader has power to get device class directly from M2TP,
  // because it "owns" the network
  const char *fromDeviceClass = m2tp_getMemberDeviceClass(fromAddress);

  // printf(...) is very handy to print formatted string,
  // see https://www.tutorialspoint.com/c_standard_library/c_function_printf.htm
  printf("%s: %.*s\n", fromDeviceClass, size, data);
}

// When a member disconnected, M2TP will tell you via this function
void onMemberQuit(m2tp_channel address)
{
  targetAddress = 0;
  printf("[INFO] Targeted device is disconnected, fallback to loopback.\n");
}
