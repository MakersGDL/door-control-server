/*
* MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT.
* The library file MFRC522.h has a wealth of useful info. Please read it.
* The functions are documented in MFRC522.cpp.
*
* Based on code Dr.Leong ( WWW.B2CQSHOP.COM )
* Created by Miguel Balboa (circuitito.com), Jan, 2012.
* Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013 (Translation to English, refactored, comments, anti collision, cascade levels.)
* Released into the public domain.
*
* Sample program showing how to read data from a PICC using a MFRC522 reader on the Arduino SPI interface.
*----------------------------------------------------------------------------- empty_skull
* Aggiunti pin per arduino Mega
* add pin configuration for arduino mega
* http://mac86project.altervista.org/
----------------------------------------------------------------------------- Nicola Coppola
* Pin layout should be as follows:
* Signal Pin Pin Pin
* Arduino Uno Arduino Mega MFRC522 board
* ------------------------------------------------------------
             PIN
* TIP120     8
* Reset      9 5 RST
* SPI SS     10 53 SDA
* SPI MOSI   11 51 MOSI
* SPI MISO   12 50 MISO
* SPI SCK    13 52 SCK

*
* The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com.
*/



#include <SPI.h>
#include <MFRC522.h>

typedef unsigned long uint32_t;

#define RFID_SS 10
#define RST_PIN 9
#define doorPin 8
                        
MFRC522 mfrc522(RFID_SS, RST_PIN);	// Create MFRC522 instance.

void setup() {
  
  pinMode(doorPin, OUTPUT);
  
  Serial.begin(9600);	// Initialize serial communications with the PC
  SPI.begin();	// Init SPI bus
  mfrc522.PCD_Init();	// Init MFRC522 card
  
  Serial.println("Set up ready...");
  Serial.println("Scan something...");
  
}

void loop() {
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // This variable help us to store the HEX info from the UID card.
  int uid[4];
  
  // Dump UID fromt the card
  Serial.print("Card UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     uid[i]= mfrc522.uid.uidByte[i];
  }
  
  uint32_t cardKey = getCardIdentifier(uid);

  Serial.println();
  Serial.print("Card Identifier: ");
  Serial.print(cardKey);
  Serial.println();
  
  // IF VALID CARDIDENTIFIER!
  
  if(cardKey == 1423244404 || cardKey == 3306424311 || cardKey == 1611156100){
    Serial.println("DOOR OPEN!!");
    
    digitalWrite(doorPin, HIGH);  
    
    delay(3000);
    
    digitalWrite(doorPin, LOW);
    
    Serial.println("DOOR CLOSED!");
  }
  
  Serial.println("-------------");
  
  // DELAY TO SET UP I GUESS!
  delay(1000);
}



// Method to get the identifier from the card UID
long getCardIdentifier(int uid[]){

  uint32_t cardIdentifier = 0;
  
  cardIdentifier = uid[3];
  cardIdentifier <<= 8; 
  cardIdentifier |= uid[2];
  cardIdentifier <<= 8; 
  cardIdentifier |= uid[1];
  cardIdentifier <<= 8; 
  cardIdentifier |= uid[0];  
  
  return cardIdentifier;
}
