/*
  Door web interface
  
  http://makerspace.makersgdl.com/
 */
 
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Servo.h>

Servo doorServo;  // create servo object to control a servo 
int pos = 0;    // variable to store the servo position

// Listen on default port 5555, the webserver on the Yún
// will forward there all the HTTP requests for us.
YunServer server;

void setup() {
  Serial.begin(9600);
  
  doorServo.attach(9);  // attaches the servo on pin 9 to the servo object 
  
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  YunClient client = server.accept();
  
  // There is a new client ? 
  if(client){
    
    // read the command 
    String command = client.readString();
    command.trim(); // kill whitespace
    Serial.println(command);
    
    // is "open" command?
    if (command == "open"){
      client.print("Please wait, opening door...");
      openDoor();
      client.println("Open!");
      delay(1000); // Wait 1000ms (1s)
    }
    
    // Close connection and free resources.
    client.stop();
  }
  
  // 1.- Read RFID
  
  // 2.- Verify in local storage 
  
  // 3.- If is not in local storage 
  if(true){
    // Make a POST request to Session Web Service
    
  }
  // Make a POST request to Log Session Web 
  
  delay(50); // Poll every 50ms
}

void openDoor(){

}
