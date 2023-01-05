#include "camera.h"

#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "Arduino.h"

#define CAMERA_MODEL_M5STACK_WIDE
#include "camera_pins.h"


#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";


typedef struct {
        httpd_req_t *req;
        size_t len;
} jpg_chunking_t;


httpd_handle_t stream_httpd = NULL;
httpd_handle_t cmd_httpd = NULL;


/* static helper functions */

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
  camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len;
    uint8_t * _jpg_buf;
    char * part_buf[64];
    static int64_t last_frame = 0;
    if(!last_frame) {
        last_frame = esp_timer_get_time();
    }

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }

    while(true){
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
            res = ESP_FAIL;
            break;
        }
        if(fb->format != PIXFORMAT_JPEG){
            bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
            if(!jpeg_converted){
                ESP_LOGE(TAG, "JPEG compression failed");
                esp_camera_fb_return(fb);
                res = ESP_FAIL;
            }
        } else {
            _jpg_buf_len = fb->len;
            _jpg_buf = fb->buf;
        }

        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if(res == ESP_OK){
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);

            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if(fb->format != PIXFORMAT_JPEG){
            free(_jpg_buf);
        }
        esp_camera_fb_return(fb);
        if(res != ESP_OK){
            break;
        }
        int64_t fr_end = esp_timer_get_time();
        int64_t frame_time = fr_end - last_frame;
        last_frame = fr_end;
        frame_time /= 1000;
        ESP_LOGI(TAG, "MJPG: %uKB %ums (%.1ffps)",
            (uint32_t)(_jpg_buf_len/1024),
            (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time);
    }

    last_frame = 0;
    return res;
}


static esp_err_t control_handler(httpd_req_t *req) {
  
}


auto camera::start_server(void) -> void {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  this->camera_setup();

  httpd_uri_t index_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri = "/control",
    .method = HTTP_POST,
    .handler = control_handler,
    .user_ctx = NULL
  };

  // start stream server one port above command server
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &index_uri);
  }

  ++config.server_port;
  ++config.ctrl_port;

  // start command server
  if (httpd_start(&cmd_httpd, &config) == ESP_OK) {
   httpd_register_uri_handler(cmd_httpd, &cmd_uri);
  }
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

  // set paramaters
  this->sensor = esp_camera_sensor_get();
  this->sensor->set_framesize(this->sensor, FRAMESIZE_SVGA);
  this->sensor->set_vflip(this->sensor, 0);
  this->sensor->set_hmirror(this->sensor, 1);
}
