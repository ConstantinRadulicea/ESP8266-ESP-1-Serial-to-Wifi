/*
* https://docs.espressif.com/projects/arduino-esp32/en/latest/
* 
*/ 
#include <Arduino.h>
#include <ESP8266WiFi.h>

#define ENABLE_SERIAL_PRINT 0
#define COMMENT_CHARACTER '%'

String wifi_ssid = "Off Limits";
String wifi_password = "J7s2tzvzKzva";
String client_hostname = "192.168.0.227";
int client_port = 6789;


void removeLastCarrerReturn(String &str){
  if (str.charAt(str.length()-1) == '\r')
  {
    str.remove(str.length() - 1, 1);
  }
}

void escapeFrontCommentCharacter(String &str, char commentCharacter){
  if (str.charAt(0) == commentCharacter)
  {
    str.remove(0, 1);
  }
}


#if ENABLE_SERIAL_PRINT == 1
#endif

void setup() {
  Serial.begin(115200);
  while (!Serial){
    delay(100);
  }

  while (!Serial.available());
  wifi_ssid = Serial.readStringUntil('\n');
  removeLastCarrerReturn(wifi_ssid);
  escapeFrontCommentCharacter(wifi_ssid, COMMENT_CHARACTER);
  #if ENABLE_SERIAL_PRINT == 1
    Serial.println(wifi_ssid);
  #endif

  while (!Serial.available());
  wifi_password = Serial.readStringUntil('\n');
  removeLastCarrerReturn(wifi_password);
  escapeFrontCommentCharacter(wifi_password, COMMENT_CHARACTER);
  #if ENABLE_SERIAL_PRINT == 1
    Serial.println(wifi_password);
  #endif
  

  while (!Serial.available());
  client_hostname = Serial.readStringUntil('\n');
  removeLastCarrerReturn(client_hostname);
  escapeFrontCommentCharacter(client_hostname, COMMENT_CHARACTER);
  #if ENABLE_SERIAL_PRINT == 1
    Serial.println(client_hostname);
  #endif

  while (!Serial.available());
  String temp_port_str = Serial.readStringUntil('\n');
  removeLastCarrerReturn(temp_port_str);
  escapeFrontCommentCharacter(temp_port_str, COMMENT_CHARACTER);
  
  client_port = temp_port_str.toInt();
  #if ENABLE_SERIAL_PRINT == 1
    Serial.println(client_port);
  #endif

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
      
    #if ENABLE_SERIAL_PRINT == 1
      Serial.print(".");
    #endif
  }
  #if ENABLE_SERIAL_PRINT == 1
    Serial.println("WiFi connected");
    Serial.print("ipv4: ");
    Serial.println(WiFi.localIP());
  #endif
}

static void connectToServer(WiFiClient& server, String hostName, uint16_t port){
  while (1)
  {
    if (!server.connect(hostName, port)) {
      delay(100);
      continue;
    }
    else{
      break;
    }
  }
}

void sendDataToServer(WiFiClient& server, String& buffer, String hostName, uint16_t port){
  int bytesRemaining, bytesWrittenNow;
    if (buffer.length() <= 0) {
      return;
    }
  if (!server.connected()) {
    connectToServer(server, hostName, port);
  }
  bytesRemaining = buffer.length();
  while (bytesRemaining > 0 && server.connected())
  {
    bytesWrittenNow = server.print(buffer);
    bytesRemaining -= bytesWrittenNow;
    buffer.remove(0, bytesWrittenNow);
  }
}

void readDataFromServer(WiFiClient& server, String& buffer, String hostName, uint16_t port){
  buffer = "";
  if (!server.connected()) {
    connectToServer(server, hostName, port);
  }
  while(server.available()) {
    buffer += server.readString();
  }
}


void loop() {
  
  WiFiClient server;
  String TXbuffer, RXbuffer;
  connectToServer(server, client_hostname, client_port);
  while (1)
  {
    if (Serial.available())
    {
      TXbuffer = Serial.readStringUntil('\n');
      TXbuffer += '\n';
      #if ENABLE_SERIAL_PRINT == 1
        Serial.print("To server: ");
        Serial.println(TXbuffer);
      #endif
      sendDataToServer(server, TXbuffer, client_hostname, client_port);
    }
    readDataFromServer(server, RXbuffer, client_hostname, client_port);

    if (RXbuffer.length() > 0)
    {
      #if ENABLE_SERIAL_PRINT == 1
        Serial.print("From server: ");
      #endif
      Serial.print(RXbuffer);
      #if ENABLE_SERIAL_PRINT == 1
        Serial.println("");
      #endif
    }
    delay(5);
  }
}
