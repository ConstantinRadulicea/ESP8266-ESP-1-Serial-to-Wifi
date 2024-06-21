Using this repository you can bridge a serial connection over wifi using an ESP8266-esp01.
The code in this repository is ment to be uploaded on the ESP.
The esp can used in 2 ways:
1) Client (default):
   ```cpp
   #define CONFIGURATION_SERVER 0
   #define CONFIGURATION_CLIENT 1
   ```
   In this configuration, the esp connects to a tcp server whose ip is sent by the serial device.

  Connection configuration on the serial device:
  ```cpp
  /*
  * Preconditions: Connect your arduino serial port to the esp serial port.
  * This program will send to ESP the informations to connect to a wifi network and a server.
  * After that, the arduino's serial port will be linked to the server on the other side
  */
  
  #include <Arduino.h>
  
  #define WIFI_SSID "Off Limits2"
  #define WIFI_PASSWORD "J7s2tzvzKzva"
  #define HOST_IPADDRESS "192.168.79.243"
  #define HOST_PORT 6789
  
  #define ESP_INIT_SEQUENCE "%SERIAL2WIFI\r\n"
  #define ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING '%'
  
  void setup() {
    Serial.begin(230400);
    while (!Serial){
      delay(100);
    }
    delay(2000);
    Serial.print(ESP_INIT_SEQUENCE);
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(WIFI_SSID));
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(WIFI_PASSWORD));
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(HOST_IPADDRESS));
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(HOST_PORT));
  }
  
  void loop() {
    Serial.println("Hello world"); // this message will be sent to the ESP via serial communication and the ESP will forward it to the server
    delay(1000);
  }
  ```

2) Server:
   ```cpp
   #define CONFIGURATION_SERVER 1
   #define CONFIGURATION_CLIENT 0
   ```
   In this configuration, the esp create a server and accepts one and only one client.

   Connection configuration on the serial device:
  ```cpp
  /*
  * Preconditions: Connect your arduino serial port to the esp serial port.
  * This program will send to ESP the informations to connect to a wifi network and a server.
  * After that, the arduino's serial port will be linked to the server on the other side
  */
  
  #include <Arduino.h>
  
  #define WIFI_SSID "Off Limits2"
  #define WIFI_PASSWORD "J7s2tzvzKzva"
  #define SERVER_PORT 6789
  
  #define ESP_INIT_SEQUENCE "%SERIAL2WIFI\r\n"
  #define ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING '%'
  
  void setup() {
    Serial.begin(230400);
    while (!Serial){
      delay(100);
    }
    delay(2000);
    Serial.print(ESP_INIT_SEQUENCE);
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(WIFI_SSID));
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(WIFI_PASSWORD));
    Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(SERVER_PORT));
  }
  
  void loop() {
    Serial.println("Hello world"); // this message will be sent to the ESP via serial communication and the ESP will forward it to the server
    delay(1000);
  }
  ```
