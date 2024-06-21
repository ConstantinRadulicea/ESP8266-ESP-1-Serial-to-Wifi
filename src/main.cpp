/*
* Copyright 2023 Constantin Dumitru Petre RĂDULICEA
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*   http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266SSDP.h>
#include <ESP8266WebServer.h>
#include <vector>

#define CONFIGURATION_CLIENT 1
#define CONFIGURATION_SERVER 0

#define SERIALPORT_BAUDRATE 230400


#define ENABLE_SERIAL_PRINT 0
#define ENABLE_WIFI_HOST_DATA_FROM_CLIENT 1
#define ENABLE_INIT_SEQUENCE 1
#define ENABLE_SSDP 1
#define ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING '%'
#define MAX_BUFFER_SIZE 16384
#define INIT_SEQUENCE "%SERIAL2WIFI\r\n"


#define LOWPOWER_MODE_TIMEOUT_ACTIVATION_MS 1000
#define LOWPOWER_MODE_LOOP_DELAY_MS 100


String wifi_ssid = "Off Limits";
String wifi_password = "J7s2tzvzKzva";
String client_hostname = "192.168.0.247";
int client_port = 6789;

#if ENABLE_SSDP == 1
  ESP8266WebServer HTTP(80);
#endif

/*
SERIAL2WIFI
Off Limits
J7s2tzvzKzva
192.168.0.227
6789
*/

void removeLastCarrerReturn(String &str){
  if (str.charAt(str.length()-1) == '\r')
  {
    str.remove(str.length() - 1, 1);
  }
}

void escapeFrontCommentCharacter(String &str, char commentCharacter){
  if (str.charAt(0) == commentCharacter) {
    str.remove(0, 1);
  }
}

String serialReadStringUntil_blocking(HardwareSerial& serialPort, char terminatorCharacter){
  char tempChar;
  String result = "";
  while (serialPort)
  {
    while (serialPort.available() > 0){
      tempChar = (char)serialPort.read();
      if (tempChar == terminatorCharacter) {
        return result;
      }
      result += tempChar;
    }
  }
  return result;
}

void setup() {
  String initializationTemp = "";
  bool initializationStringFound = false;
  char tempChar;

  Serial.setRxBufferSize(2048);
  Serial.begin(SERIALPORT_BAUDRATE);
  while (!Serial){
    delay(100);
  }
  
  #if ENABLE_SERIAL_PRINT == 1
    while (Serial.availableForWrite() <= 0){}
    Serial.println("Serial port connected");
  #endif

  #if ENABLE_INIT_SEQUENCE == 1
    while (initializationStringFound == false)
    {
      while (Serial.available() > 0){
        tempChar = (char)Serial.read();
        initializationTemp += tempChar;
        #if ENABLE_SERIAL_PRINT == 1
          Serial.print(tempChar);
        #endif

        if (initializationTemp.length() > strlen(INIT_SEQUENCE)) {
          initializationTemp.remove(0, initializationTemp.length() - (strlen(INIT_SEQUENCE)));
        }
        if (initializationTemp.length() == strlen(INIT_SEQUENCE)) {
          if (strncmp(initializationTemp.c_str(), INIT_SEQUENCE, strlen(INIT_SEQUENCE)) == 0) {
            initializationStringFound = true;
            #if ENABLE_SERIAL_PRINT == 1
              Serial.println();
              Serial.println("Initialization sequence completed!");
            #endif
            break;
          }
        }
      }
    }
  #endif
  
  #if ENABLE_WIFI_HOST_DATA_FROM_CLIENT == 1
    while (Serial.available() <= 0){}
    wifi_ssid = serialReadStringUntil_blocking(Serial, '\n');
    removeLastCarrerReturn(wifi_ssid);
    escapeFrontCommentCharacter(wifi_ssid, ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING);
    #if ENABLE_SERIAL_PRINT == 1
      Serial.println("SSID: " + wifi_ssid);
    #endif

    while (Serial.available() <= 0){}
    wifi_password = serialReadStringUntil_blocking(Serial, '\n');
    removeLastCarrerReturn(wifi_password);
    escapeFrontCommentCharacter(wifi_password, ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING);
    #if ENABLE_SERIAL_PRINT == 1
      Serial.println("Password: " + wifi_password);
    #endif
    
    #if CONFIGURATION_CLIENT == 1
      while (Serial.available() <= 0){}
      client_hostname = serialReadStringUntil_blocking(Serial, '\n');
      removeLastCarrerReturn(client_hostname);
      escapeFrontCommentCharacter(client_hostname, ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING);
      #if ENABLE_SERIAL_PRINT == 1
        Serial.println("Hostname: " + client_hostname);
      #endif
    #endif

    while (Serial.available() <= 0){}
    String temp_port_str = serialReadStringUntil_blocking(Serial, '\n');
    removeLastCarrerReturn(temp_port_str);
    escapeFrontCommentCharacter(temp_port_str, ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING);
    
    client_port = temp_port_str.toInt();
    #if ENABLE_SERIAL_PRINT == 1
      Serial.println("Port: " + String(client_port));
    #endif
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);
  WiFi.setAutoReconnect(true);

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

  #if ENABLE_SSDP == 1
    HTTP.on("/index.html", HTTP_GET, []() {
      HTTP.send(200, "text/plain", "Hello World!");
    });
    HTTP.on("/description.xml", HTTP_GET, []() {
      SSDP.schema(HTTP.client());
    });
    HTTP.begin();

    //Serial.printf("Starting SSDP...\n");
    SSDP.setSchemaURL("description.xml");
    SSDP.setHTTPPort(80);
    SSDP.setName("ESP8266-esp01");
    SSDP.setSerialNumber("001788102201");
    SSDP.setURL("index.html");
    SSDP.setModelName("esp01");
    SSDP.setModelNumber("929000226503");
    SSDP.setModelURL("https://www.espressif.com/");
    SSDP.setManufacturer("Espressif");
    SSDP.setManufacturerURL("https://www.espressif.com/");
    SSDP.begin();
  #endif
}

static void connectToServer(WiFiClient& server, String hostName, uint16_t port){
  if ((!server) || (!server.connected())) {
    if (!server.connect(hostName, port)) {
      #if ENABLE_SERIAL_PRINT == 1
        Serial.println("Connection failed!");
      #endif
    }
  }
}

void sendData(WiFiClient& server, std::vector<char>& buffer){
  size_t bytesWrittenNow;

  if (buffer.size() <= 0) {
    return;
  }
  if ((!server) || (!server.connected())) {
    return;
  }
  bytesWrittenNow = server.write(buffer.data(), buffer.size());
  buffer.erase(buffer.begin(),buffer.begin()+bytesWrittenNow);
}

void readData(WiFiClient& server, std::vector<char>& buffer){
  size_t bytesRead, bytesAvailable;

  if ((!server) || (!server.connected())) {
    return;
  }

  if (server.available() > 0)
  {
    bytesAvailable = (size_t)server.available();

    if ((buffer.size() + bytesAvailable) > MAX_BUFFER_SIZE) {
      buffer.erase(buffer.begin(),buffer.begin()+((buffer.size() + bytesAvailable) - MAX_BUFFER_SIZE));
    }
    
    buffer.resize(buffer.size() + bytesAvailable);
    bytesRead = server.readBytes(buffer.data() + buffer.size() - bytesAvailable, bytesAvailable);
    buffer.resize((int)buffer.size() - (int)((int)bytesAvailable - (int)bytesRead));
  }
}

void readDataFromSerial(HardwareSerial& serialPort, std::vector<char>& buffer){
  size_t bytesAvailableSerial, bytesReadFromSerial;
    if (serialPort.available() > 0)
    {
      bytesAvailableSerial = serialPort.available();

      if ((buffer.size() + bytesAvailableSerial) > MAX_BUFFER_SIZE) {
        buffer.erase(buffer.begin(),buffer.begin()+((buffer.size() + bytesAvailableSerial) - MAX_BUFFER_SIZE));
      }

      buffer.resize(buffer.size() + bytesAvailableSerial);
      bytesReadFromSerial = serialPort.readBytes(buffer.data() + buffer.size() - bytesAvailableSerial, bytesAvailableSerial);
      buffer.resize((int)buffer.size() - (int)((int)bytesAvailableSerial - (int)bytesReadFromSerial));
    }
}

void loop() {
  bool activeLowPowerMode = false;
  bool activityInLastIteration = false;
  unsigned int startTime;
  float inactivityTime_ms = 0.0f;
  WiFiClient tcpClient;

  #if CONFIGURATION_SERVER == 1
    WiFiServer tcpServer(client_port);
    tcpServer.begin(client_port, 1);
  #endif

  std::vector<char> TXbuffer, RXbuffer;

  TXbuffer.clear();
  RXbuffer.clear();
  TXbuffer.reserve(MAX_BUFFER_SIZE);
  RXbuffer.reserve(MAX_BUFFER_SIZE);

  startTime = millis();

  while (1)
  {
    #if ENABLE_SSDP == 1
      HTTP.handleClient();
    #endif
    
    if (activityInLastIteration == true) {
      startTime = millis();
    }
    activityInLastIteration = false;

    if (activeLowPowerMode) {
      delay(LOWPOWER_MODE_LOOP_DELAY_MS);
    }

    #if CONFIGURATION_CLIENT == 1
      connectToServer(tcpClient, client_hostname, client_port);
    #endif

    #if CONFIGURATION_SERVER == 1
      if ((!tcpClient) || (!tcpClient.connected())) {
          tcpClient = tcpServer.available();

          #if ENABLE_SERIAL_PRINT == 1
            if(tcpClient && tcpClient.connected())
              Serial.println("Client connected!");
              else {
                  Serial.println("Client NOT connected!");
              }
          #endif
      }
    #endif

    readDataFromSerial(Serial, TXbuffer);
    if (TXbuffer.size() > 0)
    {
      activityInLastIteration = true;
      #if ENABLE_SERIAL_PRINT == 1
        Serial.print("To server [" + String(TXbuffer.size()) + "]: ");
        Serial.write(TXbuffer.data(), TXbuffer.size());
        Serial.println();
      #endif
      sendData(tcpClient, TXbuffer);
    }

    readData(tcpClient, RXbuffer);

    if (RXbuffer.size() > 0)
    {
      activityInLastIteration = true;
      #if ENABLE_SERIAL_PRINT == 1
        Serial.print("From server: ");
      #endif
      RXbuffer.erase(RXbuffer.begin(),RXbuffer.begin() + Serial.write(RXbuffer.data(), RXbuffer.size()));
      #if ENABLE_SERIAL_PRINT == 1
        Serial.println();
      #endif
    }
    if (activityInLastIteration == false)
    {
      inactivityTime_ms = (float)millis() - (float)startTime;
      #if ENABLE_SERIAL_PRINT == 1
        Serial.print("Inactivity time[ms]: ");
        Serial.println(inactivityTime_ms);
      #endif
      if (inactivityTime_ms >= (float)LOWPOWER_MODE_TIMEOUT_ACTIVATION_MS) {
        activeLowPowerMode = true;
        #if ENABLE_SERIAL_PRINT == 1
          Serial.println("LowPowerMode: ENABLED");
        #endif
      }
    }
    else{
      inactivityTime_ms = 0;
      activeLowPowerMode = false;
    }
  }
}
