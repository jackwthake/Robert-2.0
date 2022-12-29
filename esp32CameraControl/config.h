#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <WebServer.h>

#define RXD2 33
#define TXD2 4

#define com_cmd Serial  // to Computer
#define com_ard Serial2 // to Robot controller

constexpr char *ssid = "esp32_robot";
constexpr char *pswd = "";

const IPAddress local_ip(192, 168, 1, 1);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

#endif