#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <WebServer.h>
#include "esp_http_server.h"

#define RXD2 33
#define TXD2 4

#define com_cmd Serial  // to Computer
#define com_ard Serial2 // to Robot controller

#define CAMERA_MODEL_M5STACK_WIDE

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

typedef struct {
        httpd_req_t *req;
        size_t len;
} jpg_chunking_t;

constexpr char *ssid = "esp32_robot";
constexpr char *pswd = "";

const IPAddress local_ip(192, 168, 1, 1);
const IPAddress gateway(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);

httpd_handle_t stream_httpd = NULL;
httpd_handle_t cmd_httpd = NULL;

camera_config_t config = { 0 };
sensor_t *sensor = NULL;

#endif