// Date : 05 June 2017
// By Sanket Sonavane
// Reference : http://www.instructables.com/id/Get-Started-with-ESP8266-Using-AT-Commands-NodeMCU/

// Board : NodeMCU v2 Amica - ESP8266 
// IDE : Arduino IDE 1.8.2 

// State : Not the best code.
// Issues
// 1. when you click ON its actually OFF and vice versa
// 2. Need to handle junk requests and get rid of them properly not every request is going to be a good request like /LED=OFF or /LED=ON

// Can be used as a reference code but not as a stable final code

#include <ESP8266WiFi.h>
 
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

int ledPin = 16; // The LED on the board is connected to Pin 16
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT); //This defines that there is an output device connected on this port i.e. the LED
  digitalWrite(ledPin, LOW); //Initialize its value to LOW or OFF state.
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  
  // Check if a client has connected
  WiFiClient client = server.available();
  
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request 
  int value = LOW;

  //Handles two requests type /LED=ON and /LED=OFF
  //TODO : Write code to handle other garbage inputs and dont process them in this loop.
  
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
    Serial.println("HIGH");
  }
  
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
    Serial.println("LOW");
  }
 
  //Set ledPin according to the request
  //digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: "); 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
 
