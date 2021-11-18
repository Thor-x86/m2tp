/*****************************************************************************************

                  Code Example for Using M2TP with MCP2515 CAN Bus Driver

Required Libraries:
- M2TP Core Library (Preloaded with this example)
- M2TP-MCP2515 Glue Library (https://github.com/Thor-x86/arduino-m2tp-mcp2515/releases)
- MCP2515 Driver Library from autowp (https://github.com/autowp/arduino-mcp2515/releases)

How to install those libraries:
1. Download those zip files
2. Open your Arduino IDE
3. On menubar, go to `Sketch -> Include Library -> Add .ZIP Library`
4. Choose downloaded zip file, repeat step 3 until all libraries added

*****************************************************************************************/

// Load M2TP library, as a device leader
#include <m2tp-leader.h>

// This is glue library for MCP2515 Driver
#include <m2tp-mcp2515.h>

// ATTENTION: Always put m2tp-leader.h on top of glue library

// NOTE: You don't have to include mcp2515.h
//       because it's already included inside m2tp-mcp2515.h

// Your unique device class,
// what is your device's role in the network?
static const char myClass[] = "Leader Device";

// Function declaration, these lines actually tells the compiler there are functions that
// will be defined somewhere at the bottom of this example code. So you don't have to put
// everything above setup and loop functions, which is more convenient.
void onNewMember(char *memberClass, m2tp_channel assignedAddress);
void onMemberQuit(m2tp_channel assignedAddress);
void onReceiveData(m2tp_channel fromAddress, m2tp_byte dataSize, const m2tp_bytes data);

// Everytime a new member connected, its address will be stored here.
// So everytime you send message, you'll send that to this address.
m2tp_channel targetAddress = 0;

void setup()
{
  // Start the serial with baudrate 9600
  Serial.begin(9600);

  // Tell M2TP which function can be use
  m2tp_onNewMemberListener = &onNewMember;
  m2tp_onAnotherMemberQuitListener = &onMemberQuit;
  m2tp_receivedListener = &onReceiveData;

  // Now, let's connect to the network!
  // This function is provided by glue library
  bool success = m2tp_connect(PA4, CAN_1000KBPS, myClass);

  // If there is a problem on MCP2515, then show error and stop
  if (!success)
  {
    Serial.println("ERROR: Failed to start the driver");
    while (true)
    {
      // Loop forever
    }
  }

  Serial.println("Connecting...");
}

void loop()
{
  // You MUST call this at the beginning of main loop
  m2tp_update();

  // Is there any message from your PC?
  if (Serial.available())
  {
    Serial.print("Me: ");
    unsigned int messageSize = Serial.available();

    // Initiate send message to targeted address
    m2tp_startSend(targetAddress);

    // This loop stream message from your PC to targeted device via M2TP + CAN Bus
    for (unsigned int i = 0; i < messageSize; i++)
    {
      // Read each letter from your PC
      char eachLetter = Serial.read();

      // Send that letter to targeted device
      m2tp_write(eachLetter);

      // Send that letter back to your PC
      Serial.write(eachLetter);
    }

    // Tell M2TP the message is finished
    m2tp_writeFinish();

    // Making sure the message has been sent back to your PC
    Serial.flush();
  }
}

// When a new member connected, M2TP will tell you via this function.
// You have to store its address to a variable if you want to send message to it.
void onNewMember(char *memberClass, m2tp_channel assignedAddress)
{
  Serial.print(memberClass);
  Serial.print(" connected with address ");
  Serial.println(assignedAddress, DEC);

  // We will always send message to the latest member for this example
  targetAddress = assignedAddress;
}

// When a member disconnected, M2TP will tell you via this function
void onMemberQuit(m2tp_channel assignedAddress)
{
  if (assignedAddress == targetAddress)
  {
    Serial.println("Targeted device is disconnected, fallback to loopback.");
    targetAddress = 0;
  }
}

// When other device send message to this device, M2TP will tell you via this function
void onReceiveData(m2tp_channel fromAddress, m2tp_byte dataSize, const m2tp_bytes data)
{
  // Leader has power to get device class directly from M2TP,
  // because it "owns" the network
  const char *fromDeviceClass = m2tp_getMemberDeviceClass(fromAddress);

  // If the address is invalid, then dump that message
  if (fromDeviceClass == nullptr)
    return;

  // Stream data from other device to your PC
  Serial.print(": ");
  for (unsigned int i = 0; i < dataSize; i++)
  {
    Serial.write(data[i]);
  }
  Serial.flush();
}

// NOTE: On leader device, there is no onConnected(...) and onDisconnected(...) function
//       because leader "owns" the network, unlike member device.
