#pragma once

#include "esp_camera.h"

// Sensor data structure
typedef struct {
    float temperature;      // in Celsius
    float humidity;        // in percentage
    int battery_voltage;   // in mV
    bool dht_valid;       // true if DHT reading is valid
    bool battery_valid;   // true if battery reading is valid
} sensor_data_t;

// Traditional sensor readings - now return data
bool read_dht_data(float* temperature, float* humidity);
int read_battery_voltage();

// Camera as a sensor
camera_fb_t* capture_camera_sensor();
void release_camera_sensor(camera_fb_t* fb);

// Unified sensor operations
sensor_data_t collect_all_sensor_data();
void log_sensor_data(const sensor_data_t* data);
