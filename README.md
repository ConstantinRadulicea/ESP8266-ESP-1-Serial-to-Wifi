```cpp
/*
* Preconditions: Connect your arduino serial port to the esp serial port.
* This program will send to ESP the informations to connect to a wifi network and a server.
* After that, the arduino's serial port will be linked to the server on the other side
*/

#include <Arduino.h>

#define DEBUG_WIFI_SSID "Off Limits2"
#define DEBUG_WIFI_PASSWORD "J7s2tzvzKzva"
#define DEBUG_HOST_IPADDRESS "192.168.79.243"
#define DEBUG_HOST_PORT 6789

#define ESP_INIT_SEQUENCE "%SERIAL2WIFI\r\n"
#define ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING '%'

void setup() {
  Serial.begin(115200);
  while (!Serial){
    delay(100);
  }
  delay(2000);
  Serial.print(ESP_INIT_SEQUENCE);
  Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(DEBUG_WIFI_SSID));
  Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(DEBUG_WIFI_PASSWORD));
  Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(DEBUG_HOST_IPADDRESS));
  Serial.println(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + String(DEBUG_HOST_PORT));
}

void loop() {
  Serial.println("Hello world"); // this message will be sent to the ESP via serial communication and the ESP will forward it to the server
  delay(1000);
}
```