#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

// Replace with your network credentials
const char* ssid = "HELLO";
const char* password = "123456789";

// Create an instance of the server
WebSocketsServer webSocket = WebSocketsServer(81);

// Ultrasonic sensor pin configuration
const int trigPin = D1;
const int echoPin = D2;

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  
  // Set up the Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Create a SoftAP
  WiFi.softAP(ssid, password);

  // Print the SoftAP IP address
  Serial.println("SoftAP IP address: " + WiFi.softAPIP().toString());

  // Start the WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  // Handle WebSocket events
  webSocket.loop();

  // Measure the ultrasonic sensor value
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Calculate distance in cm

  // Send the ultrasonic sensor value to connected clients
  String message = String(distance);
  webSocket.broadcastTXT(message);

  // Delay before next measurement
  delay(1000);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("[%u] Connected from IP address: %s\n", num, webSocket.remoteIP(num).toString().c_str());
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Received message: %s\n", num, payload);
      break;
  }
}
