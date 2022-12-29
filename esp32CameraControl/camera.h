#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "esp_camera.h"

class camera {
public:
  camera(void);
  start_server();
private:
  void camera_setup();
  void server_startup();

  camera_config_t config = { 0 };
  sensor_t *sensor = NULL;
};

#endif