#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "Race";
const char* password = "air22884";

const char* serverName = "https://krishay04.netlify.app/api/execute";


bool isInitialized = false;

WiFiClientSecure secureClient;

void setup() {
  
  Serial.begin(115200);
  delay(100);


  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 

 
  isInitialized = true;

 
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 20) { 
    delay(500);
    Serial.print(".");
    retryCount++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi.");
  }


  secureClient.setInsecure();
}

void loop() {
  
  if (!isInitialized) {
    Serial.println("Board is not initialized.");
    return;
  }

  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient https;


    https.begin(secureClient, serverName);

   
    https.addHeader("Content-Type", "application/json");

  
    String jsonData = "{\"key1\":\"value1\",\"key2\":\"value2\"}";

    int httpResponseCode = https.POST(jsonData);

 
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      String response = https.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(https.errorToString(httpResponseCode));
    }

  
    https.end();
  } else {
    Serial.println("Wi-Fi Disconnected! Attempting to reconnect...");
    WiFi.begin(ssid, password);

    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 20) { // Retry 20 times
      delay(500);
      Serial.print(".");
      retryCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nReconnected to Wi-Fi.");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nFailed to reconnect to Wi-Fi.");
    }
  }

 
  delay(10000); 
}

