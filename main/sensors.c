#include "sensors.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "dht.h"
#include "camera.h"

#define DHT_GPIO 13
#define BATTERY_ADC_GPIO ADC1_CHANNEL_6

static const char *TAG = "SENSORS";

bool read_dht_data(float* temperature, float* humidity) {
    int16_t temp = 0, hum = 0;
    if (dht_read_data(DHT_TYPE_DHT22, DHT_GPIO, &hum, &temp) == ESP_OK) {
        *temperature = temp / 10.0f;
        *humidity = hum / 10.0f;
        ESP_LOGI(TAG, "DHT Temp: %.1f°C, Hum: %.1f%%", *temperature, *humidity);
        return true;
    } else {
        ESP_LOGW(TAG, "Failed to read DHT22");
        *temperature = 0.0f;
        *humidity = 0.0f;
        return false;
    }
}

int read_battery_voltage() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(BATTERY_ADC_GPIO, ADC_ATTEN_DB_11);
    esp_adc_cal_characteristics_t chars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &chars);
    int raw = adc1_get_raw(BATTERY_ADC_GPIO);
    int mv = esp_adc_cal_raw_to_voltage(raw, &chars) * 2; // factor of 2 from voltage divider
    ESP_LOGI(TAG, "Battery Voltage: %dmV", mv);
    return mv;
}

camera_fb_t* capture_camera_sensor() {
    ESP_LOGI(TAG, "Capturing camera sensor data...");
    return capture_image();
}

void release_camera_sensor(camera_fb_t* fb) {
    release_image(fb);
}

sensor_data_t collect_all_sensor_data() {
    ESP_LOGI(TAG, "Collecting all sensor data...");
    
    sensor_data_t data = {0};
    
    // Read environmental sensors
    data.dht_valid = read_dht_data(&data.temperature, &data.humidity);
    
    // Read battery voltage
    data.battery_voltage = read_battery_voltage();
    data.battery_valid = (data.battery_voltage > 0);
    
    ESP_LOGI(TAG, "All sensor data collected");
    return data;
}

void log_sensor_data(const sensor_data_t* data) {
    ESP_LOGI(TAG, "=== SENSOR DATA SUMMARY ===");
    if (data->dht_valid) {
        ESP_LOGI(TAG, "Temperature: %.1f°C", data->temperature);
        ESP_LOGI(TAG, "Humidity: %.1f%%", data->humidity);
    } else {
        ESP_LOGI(TAG, "Temperature: INVALID");
        ESP_LOGI(TAG, "Humidity: INVALID");
    }
    
    if (data->battery_valid) {
        ESP_LOGI(TAG, "Battery: %dmV", data->battery_voltage);
    } else {
        ESP_LOGI(TAG, "Battery: INVALID");
    }
    ESP_LOGI(TAG, "========================");
}
