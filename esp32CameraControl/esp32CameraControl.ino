#include <WiFi.h>

#include "config.h"
#include "camera.h"

static camera cam;

/* handle incoming connections */
auto on_connection(void) -> void {
  httpd.send(200, "text/html", "hello world!");
}


/* startup */
auto setup() -> void {
  // setup COMs
  com_cmd.begin(9600);
  com_ard.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // setup wifi network
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pswd);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  // startup server
  cam.start_server();
}


/* main loop */
auto loop() -> void {}