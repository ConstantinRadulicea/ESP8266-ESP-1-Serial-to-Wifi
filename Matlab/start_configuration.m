% delete(device)
clc;
clear;
close all;

baudRate = 230400;


device = serialport("COM4",baudRate);
%configureTerminator(device,"CR/LF");
flush(device);
%configureCallback(device,"terminator",@read_callback_serialport);
configureCallback(device,"byte",1,@read_callback_serialport)

device.UserData.recved = char('');

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
  SSDP_DEVICETYPE = "urn:schemas-upnp-org:device:NXP-CAR:1";
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

    device.write(ESP_INIT_SEQUENCE, "char");
    device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + CONFIGURATION);
    device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + WIFI_SSID);
    device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + WIFI_PASSWORD);
    if ENABLE_CLIENT == 1
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + HOST_IPADDRESS);
    end
    device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + HOST_PORT);

    if ENABLE_SSDP == 1
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_ENABLE_STRING);
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_DEVICETYPE);
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_NAME);
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_SERIALNUMBER);
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MODELNAME);
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MODELNUMBER);
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MODELURL);
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MANUFACTURER);
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + SSDP_MANUFACTURERURL);
    else
      device.writeline(ESCAPED_CHARACTER_AT_BEGINNING_OF_STRING + "DISABLE_SSDP");
    end