#include <WiFi.h>
#include <WebServer.h>

#include "config.h"

WebServer httpd(80);

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
  WiFi.softAP(ssid, pswd);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  // add routes and begin serving
  httpd.on("/", on_connection);
  httpd.begin();
}


/* main loop */
auto loop() -> void {
  httpd.handleClient();
}