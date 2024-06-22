% delete(arduinoObj)
clc;
clear;
close all;


arduinoObj = serialport("COM4",230400);
configureTerminator(arduinoObj,"CR/LF");
flush(arduinoObj);
configureCallback(arduinoObj,"terminator",@read_callback_serialport);


  ENABLE_SERVER = 1;
  ENABLE_CLIENT = 0;
  ENABLE_SSDP = 1;

  
  ESP_INIT_SEQUENCE = "%SERIAL2WIFI" + sprintf('\r') + newline;
  ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING = '%';

  WIFI_SSID = "Off Limits";
  WIFI_PASSWORD = "J7s2tzvzKzva";
  HOST_IPADDRESS = "192.168.0.247";
  HOST_PORT = "6789";

  SSDP_ENABLE_STRING = "ENABLE_SSDP";
  SSDP_NAME = "name";
  SSDP_SERIALNUMBER = "Serial Number";
  SSDP_MODELNAME = "Model Name";
  SSDP_MODELNUMBER = "Model Number";
  SSDP_MODELURL = "Model URL";
  SSDP_MANUFACTURER = "Manufacturer";
  SSDP_MANUFACTURERURL = "Manufacturer URL";

  if ENABLE_SERVER == 1
    CONFIGURATION = "SERVER";
  end
  
  if ENABLE_CLIENT == 1
    CONFIGURATION = "CLIENT";
  end

    arduinoObj.write(ESP_INIT_SEQUENCE, "char");
    arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + CONFIGURATION);
    arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + WIFI_SSID);
    arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + WIFI_PASSWORD);
    if ENABLE_CLIENT == 1
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + HOST_IPADDRESS);
    end
    arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + HOST_PORT);

    if ENABLE_SSDP == 1
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_ENABLE_STRING);
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_NAME);
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_SERIALNUMBER);
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MODELNAME);
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MODELNUMBER);
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MODELURL);
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MANUFACTURER);
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MANUFACTURERURL);
    else
      arduinoObj.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + "DISABLE_SSDP");
    end