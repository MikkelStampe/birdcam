#include "camera.h"
#include "esp_camera.h"
#include "esp_log.h"

static const char *TAG = "CAMERA";

void init_camera() {
    camera_config_t config = {
        .pin_pwdn  = -1,
        .pin_reset = -1,
        .pin_xclk = 0,
        .pin_sscb_sda = 26,
        .pin_sscb_scl = 27,
        .pin_d7 = 35, .pin_d6 = 34, .pin_d5 = 39, .pin_d4 = 36,
        .pin_d3 = 21, .pin_d2 = 19, .pin_d1 = 18, .pin_d0 = 5,
        .pin_vsync = 25, .pin_href = 23, .pin_pclk = 22,
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_SVGA,
        .jpeg_quality = 12,
        .fb_count = 1
    };

    if (esp_camera_init(&config) != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed");
    } else {
        ESP_LOGI(TAG, "Camera initialized");
    }
}

camera_fb_t* capture_image() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Failed to capture image");
        return NULL;
    }
    
    ESP_LOGI(TAG, "Image captured, size: %zu bytes", fb->len);
    return fb;
}

void release_image(camera_fb_t* fb) {
    if (fb) {
        esp_camera_fb_return(fb);
        ESP_LOGI(TAG, "Image buffer released");
    }
}
