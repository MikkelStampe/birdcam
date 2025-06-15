#include "upload.h"
#include "wifi_manager.h"
#include "esp_log.h"
#include "esp_mac.h"             // ✅ Required now in ESP-IDF 5+
#include "esp_http_client.h"
#include "esp_timer.h"
#include <stdio.h>
#include <string.h>

static const char *TAG = "UPLOAD";

void upload_image(const uint8_t* image_data, size_t image_size) {
    ESP_LOGI(TAG, "upload_image() called");

    // Check if Wi-Fi is connected before attempting upload
    if (!wifi_manager_is_connected()) {
        ESP_LOGE(TAG, "Wi-Fi not connected, cannot upload image");
        return;
    }

    if (!image_data || image_size == 0) {
        ESP_LOGE(TAG, "Invalid image data: image_data=%p, image_size=%zu", image_data, image_size);
        return;
    }

    // Log the first few bytes for inspection (careful not to overflow UART)
    ESP_LOGI(TAG, "Image size: %zu bytes", image_size);
    for (int i = 0; i < 8 && i < image_size; i++) {
        ESP_LOGI(TAG, "image_data[%d] = 0x%02X", i, image_data[i]);
    }

    ESP_LOGI(TAG, "Starting HTTP upload...");

    esp_http_client_config_t config = {
        .url = "http://192.168.1.124:8080/upload-bird",
        .method = HTTP_METHOD_POST,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to init HTTP client");
        return;
    }

    // SAFER: Use esp_http_client_open + write instead of copying the whole image
    esp_err_t err = esp_http_client_open(client, image_size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return;
    }

    int written = esp_http_client_write(client, (const char *)image_data, image_size);
    if (written < 0) {
        ESP_LOGE(TAG, "Failed to write image data");
    } else {
        ESP_LOGI(TAG, "Successfully wrote %d bytes", written);
    }

    esp_http_client_fetch_headers(client);
    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    ESP_LOGI(TAG, "Upload complete");
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
        .url = "http://192.168.1.124:8080/sensor-data",
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
