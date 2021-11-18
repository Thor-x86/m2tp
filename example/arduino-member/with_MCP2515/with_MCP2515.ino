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

// Load M2TP library, as a device member
#include <m2tp-member.h>

// This is glue library for MCP2515 Driver
#include <m2tp-mcp2515.h>

// ATTENTION: Always put m2tp-member.h on top of glue library

// NOTE: You don't have to include mcp2515.h
//       because it's already included inside m2tp-mcp2515.h

// Your unique device class,
// what is your device's role in the network?
static const char myClass[] = "Example #1";

// Function declaration, these lines actually tells the compiler there are functions that
// will be defined somewhere at the bottom of this example code. So you don't have to put
// everything above setup and loop functions, which is more convenient.
void onNewMember(char *memberClass, m2tp_channel assignedAddress);
void onMemberQuit(m2tp_channel assignedAddress);
void onReceiveData(m2tp_channel fromAddress, m2tp_byte dataSize, const m2tp_bytes data);
void onConnected(m2tp_channel address);
void onDisconnected();

void setup()
{
  // Start the serial with baudrate 9600
  Serial.begin(9600);

  // Tell M2TP which function can be use
  m2tp_onNewMemberListener = &onNewMember;
  m2tp_onAnotherMemberQuitListener = &onMemberQuit;
  m2tp_receivedListener = &onReceiveData;
  m2tp_onConnected = &onConnected;
  m2tp_onDisconnected = &onDisconnected;

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
    // Is the device currently connected?
    if (m2tp_isConnected())
    {
      Serial.print("Me: ");
      unsigned int messageSize = Serial.available();

      // Initiate send message to leader
      m2tp_startSend(0); // 0 is address of leader device

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

    // ...or this device is not connected?
    else
    {
      // Dump all messages from your PC
      while (Serial.available())
      {
        Serial.read();
      }
    }
  }
}

// When a new member connected, M2TP will tell you via this function.
// You have to store its address to a variable if you want to send message to it.
void onNewMember(char *memberClass, m2tp_channel assignedAddress)
{
  Serial.print(memberClass);
  Serial.print(" connected with address ");
  Serial.println(assignedAddress, DEC);

  // Uncomment below to store address of "Expected Device Class" to yourVariableName,
  // so you can receive message from it and knowing if that device just disconnected.
  /*
  if (strcmp(memberClass, "Expected Device Class") == 0)
  {
    yourVariableName = assignedAddress;
  }
  */
}

// When a member disconnected, M2TP will tell you via this function
void onMemberQuit(m2tp_channel assignedAddress)
{
  // This is an example to mark address in yourVariableName,
  // also don't forget to change "Expected Device Class"
  /*
  if (assignedAddress == yourVariableName)
  {
    Serial.print("Expected Device Class");
    Serial.println(" is disconnected");
    yourVariableName = 0;
  }
  */
}

// When other device send message to this device, M2TP will tell you via this function
void onReceiveData(m2tp_channel fromAddress, m2tp_byte dataSize, const m2tp_bytes data)
{
  if (fromAddress == 0)
  {
    Serial.print("Leader Device"); // Change this to your leader device class
  }

  // Uncomment below if you want to listen from a device address that stored in yourVariableName,
  // don't forget to change "Expected Device Class"
  /*
  else if (fromAddress == yourVariableName)
  {
    Serial.print("Expected Device Class");
  }
  */

  // Discards unknown address
  else
  {
    return;
  }

  // Stream data from other device to your PC
  Serial.print(": ");
  for (unsigned int i = 0; i < dataSize; i++)
  {
    Serial.write(data[i]);
  }
  Serial.flush();
}

// When this device just connected, M2TP will tell you via this function
void onConnected(m2tp_channel address)
{
  Serial.print("Connected as address 0x");
  Serial.println(address, HEX);
}

// When this device just disconnected, M2TP will tell you via this function
void onDisconnected()
{
  Serial.println("Disconnected!");

  // Don't forget to clear stored addresses
  /*
  yourVariableName = 0;
  yourVariableName1 = 0;
  yourVariableName2 = 0;
  yourVariableName3 = 0;
  */
}
