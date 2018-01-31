#include "SX1278.h"
#include <SPI.h>

#define LORA_MODE  4
#define LORA_CHANNEL  CH_6_BW_125
#define LORA_ADDRESS  4

#define LORA_LED  2

int e;
char my_packet[100];

void setup()
{
  pinMode(LORA_LED, OUTPUT);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  // Print a start message
  Serial.println(F("sx1278 module and Arduino: receiving packets"));

  // Power ON the module
  if (sx1278.ON() == 0) {
    Serial.println(F("Setting power ON: SUCCESS "));
  } else {
    Serial.println(F("Setting power ON: ERROR "));
  }

  // Set transmission mode and print the result
  if (sx1278.setMode(LORA_MODE) == 0) {
    Serial.println(F("Setting Mode: SUCCESS "));
  } else {
    Serial.println(F("Setting Mode: ERROR "));
  }

  // Set header
  if (sx1278.setHeaderON() == 0) {
    Serial.println(F("Setting Header ON: SUCCESS "));
  } else {
    Serial.println(F("Setting Header ON: ERROR "));
  }

  // Select frequency channel
  if (sx1278.setChannel(LORA_CHANNEL) == 0) {
    Serial.println(F("Setting Channel: SUCCESS "));
  } else {
    Serial.println(F("Setting Channel: ERROR "));
  }

  // Set CRC
  if (sx1278.setCRC_ON() == 0) {
    Serial.println(F("Setting CRC ON: SUCCESS "));
  } else {
    Serial.println(F("Setting CRC ON: ERROR "));
  }

  // Select output power (Max, High, Intermediate or Low)
  if (sx1278.setPower('M') == 0) {
    Serial.println(F("Setting Power: SUCCESS "));
  } else {
    Serial.println(F("Setting Power: ERROR "));
  }

  // Set the node address and print the result
  if (sx1278.setNodeAddress(LORA_ADDRESS) == 0) {
    Serial.println(F("Setting node address: SUCCESS "));
  } else {
    Serial.println(F("Setting node address: ERROR "));
  }

  // Print a success message
  Serial.println(F("sx1278 configured finished"));
  Serial.println();
}

void validatePacketsReceive(){
  
    uint8_t last_packetNumber;
    int packetLost;
    uint8_t voltas;
    int totalPackets;
    int packetLoss;
    totalPackets++;
    uint8_t e;
    
    if(sx1278._packetNumber!=255){
       e=sx1278._packetNumber-last_packetNumber;
       
       if(e>1){
          packetLost=packetLost+e;
       }
       
    }
    
    if(sx1278._packetNumber == 255 || ((last_packetNumber==254 || last_packetNumber==253 || last_packetNumber==252 ) && sx1278._packetNumber==0)){  //needed other verification
        voltas++;
        Serial.println("Voltas: ");
        Serial.print(voltas);
    }
    
    if(voltas == 4){
      packetLoss=(packetLost/totalPackets);
      Serial.println("--------------------------------------------------------------------------------------");
      Serial.println("Total Packets received in this round: ");
      Serial.print(totalPackets);
      Serial.println("Packets Lost: ");
      Serial.print(packetLost);
      Serial.println("Packet Loss: ");
      Serial.print(packetLoss);
      Serial.println("--------------------------------------------------------------------------------------");

      voltas=0;
      packetLoss=0;
      totalPackets=0;
      packetLost=0;
   
    }

    last_packetNumber=sx1278._packetNumber;
  
}

void loop(void)
{
  // Receive message for 10 seconds
  e = sx1278.receivePacketTimeout(10000);
  if (e == 0) {
    digitalWrite(LORA_LED, HIGH);
    delay(500);
    digitalWrite(LORA_LED, LOW);
      
    //Serial.println(F("Package received!"));

    for (unsigned int i = 0; i < sx1278.packet_received.length; i++) {
      my_packet[i] = (char)sx1278.packet_received.data[i];
    }
    
//    Serial.print(F("Message: Source | Packet Number ! Packet Lenght | Payload "));
    Serial.print(F("Source | Packet Number ! Packet Lenght | Payload -> "));
    Serial.print(sx1278.packet_received.src);
    Serial.print(" | ");
    Serial.print(sx1278.packet_received.packnum);
    Serial.print(" | ");
    Serial.print(sx1278.packet_received.length);
    Serial.println(" ");
//    Serial.print(" | ");
//    Serial.println(my_packet);
    sx1278.getSNR();
    sx1278.getRSSI();
    
    validatePacketsReceive();
    
  } 
  else {
    Serial.println(F("Did not receive a message in last 10 seconds\n"));
  }
}
