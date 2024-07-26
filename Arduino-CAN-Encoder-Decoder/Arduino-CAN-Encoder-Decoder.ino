#include "CAN_ENCODE_DECODE.h"
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(5);                               // Set CS to pin 10

// Define the IDs you want to filter (should be in the range 0x700 to 0x7FF)
const uint32_t FILTER_IDS[] = {0x700, 0x710, 0x720}; // Add your desired IDs here
const int FILTER_COUNT = sizeof(FILTER_IDS) / sizeof(FILTER_IDS[0]);

void setup()
{
  Serial.begin(115200);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  // Initialize mask and filter to allow only receipt of 0x7xx CAN IDs
  CAN0.init_Mask(0, 0, 0x07000000);              // Init first mask...
  CAN0.init_Mask(1, 0, 0x07000000);              // Init second mask...

  for (uint8_t i = 0; i < 6; ++i) {
    CAN0.init_Filt(i, 0, 0x07000000);           // Init filters
  }
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example with ID Filter...");
}

bool isIdInFilter(uint32_t id) {
  for (int i = 0; i < FILTER_COUNT; i++) {
    if (id == FILTER_IDS[i]) {
      return true;
    }
  }
  return false;
}

void loop()
{
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    // The hardware filter will only let through 0x7xx IDs, but we'll still check our software filter
    if (isIdInFilter(rxId & 0x1FFFFFFF)) {
      if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
        sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
      else
        sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
    
      Serial.print(msgString);
    
      if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
        sprintf(msgString, " REMOTE REQUEST FRAME");
        Serial.print(msgString);
      } else {
        for(byte i = 0; i<len; i++){
          sprintf(msgString, " 0x%.2X", rxBuf[i]);
          Serial.print(msgString);
        }
        
        // Example of decoding a signal (adjust parameters as needed)
        float decodedValue = decode(rxBuf, 0, 16, true, false, 0.1, 0);
        Serial.print(" Decoded Value: ");
        Serial.print(decodedValue);
      }
          
      Serial.println();
    }
  }
}
