#include "camera.h"

#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "Arduino.h"

#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"

#define CAMERA_MODEL_M5STACK_WIDE
#include "camera_pins.h"

/* static helper functions */

static void rgb_print(dl_matrix3du_t *image_matrix, uint32_t color, const char *str) {
    fb_data_t fb;
    fb.width = image_matrix->w;
    fb.height = image_matrix->h;
    fb.data = image_matrix->item;
    fb.bytes_per_pixel = 3;
    fb.format = FB_BGR888;
    fb_gfx_print(&fb, (fb.width - (strlen(str) * 14)) / 2, 10, color, str);
}


static int rgb_printf(dl_matrix3du_t *image_matrix, uint32_t color, const char *format, ...) {
    char loc_buf[64];
    char *temp = loc_buf;
    int len;
    va_list arg;
    va_list copy;
    
    va_start(arg, format);
    va_copy(copy, arg);
    len = vsnprintf(loc_buf, sizeof(loc_buf), format, arg);
    va_end(copy);

    if (len >= sizeof(loc_buf)) {
        temp = (char *)malloc(len + 1);
        if (temp == NULL) {
            return 0;
        }
    }
    
    vsnprintf(temp, len + 1, format, arg);
    va_end(arg);
    rgb_print(image_matrix, color, temp);

    if (len > 64) {
        free(temp);
    }

    return len;
}


static size_t jpg_encode_stream(void *arg, size_t index, const void *data, size_t len) {
    jpg_chunking_t *j = (jpg_chunking_t *)arg;
    if (!index) {
        j->len = 0;
    }

    if (httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK) {
        return 0;
    }

    j->len += len;
    return len;
}


/* request handlers */
static esp_err_t stream_handler(httpd_req_t *req) {

}


static esp_err_t control_handler(httpd_req_t *req) {
  
}


/* Camera Class implementation */
camera::camera(void) { }


auto camera::start_server(void) -> void {
  this->camera_setup();


}


auto camera::camera_setup(void) -> void {
  this->config.ledc_channel = LEDC_CHANNEL_0;
  this->config.ledc_timer = LEDC_TIMER_0;
  this->config.pin_d0 = Y2_GPIO_NUM;
  this->config.pin_d1 = Y3_GPIO_NUM;
  this->config.pin_d2 = Y4_GPIO_NUM;
  this->config.pin_d3 = Y5_GPIO_NUM;
  this->config.pin_d4 = Y6_GPIO_NUM;
  this->config.pin_d5 = Y7_GPIO_NUM;
  this->config.pin_d6 = Y8_GPIO_NUM;
  this->config.pin_d7 = Y9_GPIO_NUM;
  this->config.pin_xclk = XCLK_GPIO_NUM;
  this->config.pin_pclk = PCLK_GPIO_NUM;
  this->config.pin_vsync = VSYNC_GPIO_NUM;
  this->config.pin_href = HREF_GPIO_NUM;
  this->config.pin_sscb_sda = SIOD_GPIO_NUM;
  this->config.pin_sscb_scl = SIOC_GPIO_NUM;
  this->config.pin_pwdn = PWDN_GPIO_NUM;
  this->config.pin_reset = RESET_GPIO_NUM;
  this->config.xclk_freq_hz = 20000000;
  this->config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    this->config.frame_size = FRAMESIZE_UXGA;
    this->config.jpeg_quality = 10;
    this->config.fb_count = 2;
  } else {
    this->config.frame_size = FRAMESIZE_SVGA;
    this->config.jpeg_quality = 12;
    this->config.fb_count = 1;
  }

  // initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  this->sensor = esp_camera_sensor_get();
  this->sensor->set_framesize(this->sensor, FRAMESIZE_SVGA);
  this->sensor->set_vflip(this->sensor, 0);
  this->sensor->set_hmirror(this->sensor, 1);
}
