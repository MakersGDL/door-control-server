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

#include <Bridge.h>
//#include <YunServer.h>
//#include <YunClient.h>
#include <Process.h>

typedef unsigned long uint32_t;

#define RFID_SS 10
#define RST_PIN 9
#define doorPin 8
                        
MFRC522 mfrc522(RFID_SS, RST_PIN);	// Create MFRC522 instance.

//Servo doorServo;  // create servo object to control a servo 
//int pos = 0;    // variable to store the servo position

// Listen on default port 5555, the webserver on the Yún
// will forward there all the HTTP requests for us.
//YunServer server;

// You can find this in your service dashboard
//const char *serverName = "arduinouno.azure-mobile.net";
const char *serverName = "makersgdl.com/makerspace/door/rfid";

// Key 
const char *key = "YaRWxxxxzKGRxxxxLPiNxxxxXYOvxxxx";

Process proc;

void setup() {
  
  pinMode(doorPin, OUTPUT);
  
  Serial.begin(9600);	// Initialize serial communications with the PC
  SPI.begin();	// Init SPI bus
  mfrc522.PCD_Init();	// Init MFRC522 card
  
  Serial.println("Set up ready...");
  Serial.println("Scan something...");
  
  // Bridge startup
//  pinMode(13, OUTPUT);
//  digitalWrite(13, LOW);
  Bridge.begin();
//  digitalWrite(13, HIGH);
  
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
//  server.listenOnLocalhost();
//  server.begin();
}

void loop() {
  // TEST!!!!! Make a POST request to Session Web Service
    send_request(1423244404);
    wait_response();
    isValidKey();
    delay(3000);
  
 // Serial.println("Looping...");
  
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
      /**
     * Validate with Web Server
     *
     */
    
  // Make a POST request to Session Web Service
  send_request(cardKey);
  wait_response();
  if(isValidKey()){
      openDoor();
  } else {
    if(cardKey == 1423244404 || cardKey == 3306424311 || cardKey == 1611156100 || cardKey == 1423714916 || cardKey == 1610997476 || cardKey == 1611485620 || cardKey == 1611295220 || cardKey == 1423421332 || cardKey == 1423916516 || cardKey == 1423536196 || cardKey == 1423673556){
      openDoor();
    }
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

/*
 * Send an HTTP POST request to the Azure Mobile Service data API
 */
 
void send_request(uint32_t value)
{
  Serial.print("sending request: ");
  Serial.println(value);
  char *buffer = "";
  proc.begin("curl");
//  proc.addParameter("-k");
  proc.addParameter("-X");
  proc.addParameter("POST");
//  proc.addParameter("-H");
//  proc.addParameter("Content-Type:application/json");
 
  // Azure Mobile Services application key
//  proc.addParameter("-H");
//  sprintf(buffer, "X-Parse-REST-API-Key:%s", key);
//  proc.addParameter(buffer);
 
  // POST body
  proc.addParameter("--data");
//  sprintf(buffer, "{\"cardKey\": %d}", value);
  sprintf(buffer, "cardKey=%lu", value);
  proc.addParameter(buffer);
  sprintf(buffer, "", "");
//  Serial.flush();
//  Serial.println("cardKey="+ value);
  
  // POST URI
  proc.addParameter("http://makersgdl.com/makerspace/door/rfid");
  // Run the command synchronously
  proc.run();
}

void wait_response(){
  int wait = 0;
  while (!proc.available()) {
    Serial.println("Waiting response...");
    if(wait == 50){ break; }
    delay(100);
    wait += 1;
  }
}

String result = "";
bool isValidKey(){
  result = "";
  Serial.println(result);
  while (proc.available()) {
    char c = proc.read();
    Serial.println(c);
    result += c;
  }
  result = "";
    Serial.println();
    Serial.print("Response: ");
    Serial.println(result);
    //Serial.flush();
    
    if(result == "true"){
      Serial.println("Must be open door");
        return true;
    } else {
      Serial.println("Wrong");
        return false;      
    }
}

void openDoor(){
  Serial.println("DOOR OPEN!!");
    
    digitalWrite(doorPin, HIGH);  
    
    delay(2000);
    
    digitalWrite(doorPin, LOW);
    
    Serial.println("DOOR CLOSED!");
}
