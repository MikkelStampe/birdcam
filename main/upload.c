#include "upload.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include <stdio.h>
#include <string.h>

static const char *TAG = "UPLOAD";

void upload_image(const uint8_t* image_data, size_t image_size) {
    if (!image_data || image_size == 0) {
        ESP_LOGE(TAG, "Invalid image data provided");
        return;
    }

    esp_http_client_config_t config = {
        .url = "http://your-server.local/upload",
        .method = HTTP_METHOD_POST,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/octet-stream");
    esp_http_client_set_post_field(client, (const char *)image_data, image_size);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Image uploaded successfully, size: %zu bytes", image_size);
    } else {
        ESP_LOGE(TAG, "HTTP POST failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

void upload_sensor_data(const sensor_data_t* sensor_data) {
    if (!sensor_data) {
        ESP_LOGE(TAG, "Invalid sensor data provided");
        return;
    }

    // Create JSON payload
    char json_buffer[256];
    int json_len = snprintf(json_buffer, sizeof(json_buffer),
        "{\n"
        "  \"temperature\": %.1f,\n"
        "  \"humidity\": %.1f,\n"
        "  \"battery_voltage\": %d,\n"
        "  \"dht_valid\": %s,\n"
        "  \"battery_valid\": %s,\n"
        "  \"timestamp\": %lld\n"
        "}",
        sensor_data->temperature,
        sensor_data->humidity,
        sensor_data->battery_voltage,
        sensor_data->dht_valid ? "true" : "false",
        sensor_data->battery_valid ? "true" : "false",
        esp_timer_get_time() / 1000000LL  // Unix timestamp in seconds
    );

    if (json_len >= sizeof(json_buffer)) {
        ESP_LOGE(TAG, "JSON buffer too small");
        return;
    }

    ESP_LOGI(TAG, "Uploading sensor data: %s", json_buffer);

    esp_http_client_config_t config = {
        .url = "http://your-server.local/sensor-data",
        .method = HTTP_METHOD_POST,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, json_buffer, json_len);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Sensor data uploaded successfully");
    } else {
        ESP_LOGE(TAG, "Sensor data upload failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}
