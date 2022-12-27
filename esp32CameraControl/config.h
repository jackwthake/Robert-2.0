#ifndef __CONFIG_H__
#define __CONFIG_H__

#define RXD2 33
#define TXD2 4

#define com_cmd Serial  // to Computer
#define com_ard Serial2 // to Robot controller

const char *ssid = "esp32_robot";
const char *pswd = "";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#endif