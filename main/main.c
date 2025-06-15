#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "camera.h"
#include "sensors.h"
#include "upload.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIR_GPIO GPIO_NUM_12
#define TAG "MAIN"

void app_main(void) {
    ESP_LOGI(TAG, "Waking from deep sleep...");

    // Initialize camera
    init_camera();
    
    // Capture image using camera sensor
    camera_fb_t* image = capture_camera_sensor();
    if (image) {
        // Upload the captured image
        // upload_image(image->buf, image->len);
        // Release the image buffer
        release_camera_sensor(image);
    } else {
        ESP_LOGE(TAG, "Failed to capture image");
    }
    
    // Collect sensor data (temperature, humidity, battery)
    /* sensor_data_t sensor_data = collect_all_sensor_data();
    
    // Log sensor data summary
    log_sensor_data(&sensor_data);
    
    // Upload sensor data
    upload_sensor_data(&sensor_data); */

    /* ESP_LOGI(TAG, "Entering deep sleep...");
    esp_sleep_enable_ext0_wakeup(PIR_GPIO, 1);
    esp_deep_sleep_start(); */

    for (int i = 30; i >= 0; i--) {
        ESP_LOGI(TAG, "Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    ESP_LOGI(TAG, "Restarting now.\n");

    fflush(stdout);
    esp_restart();
}
