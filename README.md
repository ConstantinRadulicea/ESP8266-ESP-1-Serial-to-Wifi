```ruby
/*
* Preconditions: Connect your arduino serial port to the esp serial port.
* This program will send to ESP the informations to connect to a wifi network and a server.
* After that, the arduino's serial port will be linked to the server on the other side
*/

#include <Arduino.h>

#define DEBUG_WIFI_SSID "Off Limits"
#define DEBUG_WIFI_PASSWORD "J7s2tzvzKzva"
#define DEBUG_HOST_IPADDRESS "192.168.0.227"
#define DEBUG_HOST_PORT 6789

#define ESP_MSG_INIT_CHARACTER '%'

void setup() {
  Serial.begin(115200);
  while (!Serial){
    delay(100);
  }
  Serial.println(ESP_MSG_INIT_CHARACTER + String(DEBUG_WIFI_SSID));
  Serial.println(ESP_MSG_INIT_CHARACTER + String(DEBUG_WIFI_PASSWORD));
  Serial.println(ESP_MSG_INIT_CHARACTER + String(DEBUG_HOST_IPADDRESS));
  Serial.println(ESP_MSG_INIT_CHARACTER + String(DEBUG_HOST_PORT));
}

void loop() {
  Serial.println("Hello world") // this message will be sent to the ESP via serial communication and the ESP will forward it to the server
}
```
