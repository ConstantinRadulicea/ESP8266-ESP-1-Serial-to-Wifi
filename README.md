Using this repository you can bridge a serial connection over wifi using an ESP8266-esp01.
The code in this repository is ment to be uploaded on the ESP.
The esp can used in 2 ways:
1) CLIENT: In this configuration, the esp connects to a tcp server whose ip is sent by the serial device.

2) SERVER: In this configuration, the esp create a server and accepts one and only one client.


  Connection configuration on the serial device:
  ```cpp
  /*
  * Preconditions: Connect your arduino serial port to the esp serial port.
  * This program will send to ESP the informations to connect to a wifi network and a server.
  * After that, the arduino's serial port will be linked to the server on the other side
  */
  
  #include <Arduino.h>
  #define ENABLE_SERVER 1
  #define ENABLE_CLIENT 0
  #define ENABLE_SSDP 1

  
  #define ESP_INIT_SEQUENCE "%SERIAL2WIFI\r\n"
  #define ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING '%'

  #define WIFI_SSID "Off Limits"
  #define WIFI_PASSWORD "J7s2tzvzKzva"
  #define HOST_IPADDRESS "192.168.0.247"
  #define HOST_PORT 6789

  #define SSDP_ENABLE_STRING "ENABLE_SSDP"
  #define SSDP_NAME "name"
  #define SSDP_SERIALNUMBER "Serial Number"
  #define SSDP_MODELNAME "Model Name"
  #define SSDP_MODELNUMBER "Model Number"
  #define SSDP_MODELURL "Model URL"
  #define SSDP_MANUFACTURER "Manufacturer"
  #define SSDP_MANUFACTURERURL "Manufacturer URL"

  #if ENABLE_SERVER == 1
    #define CONFIGURATION "SERVER"
  #endif
  
  #if ENABLE_CLIENT == 1
    #define CONFIGURATION "CLIENT"
  #endif
  
  void setup() {
    Serial.begin(230400);
    while (!Serial){
      delay(100);
    }
    delay(2000);
    Serial.print(ESP_INIT_SEQUENCE);
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(CONFIGURATION));
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(WIFI_SSID));
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(WIFI_PASSWORD));
    #if ENABLE_CLIENT == 1
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(HOST_IPADDRESS));
    #endif
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(HOST_PORT));

    #if ENABLE_SSDP == 1
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SSDP_ENABLE_STRING));
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SSDP_NAME));
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SSDP_SERIALNUMBER));
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SSDP_MODELNAME));
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SSDP_MODELNUMBER));
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SSDP_MODELURL));
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SSDP_MANUFACTURER));
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SSDP_MANUFACTURERURL));
    #else
      Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String("DISABLE_SSDP"));
    #endif
  }
  
  void loop() {
    Serial.println("Hello world"); // this message will be sent to the ESP via serial communication and the ESP will forward it to the server
    delay(1000);
  }
  ```
