#pragma once

#include "esp_camera.h"
#include "sensors.h"

void upload_image(const uint8_t* image_data, size_t image_size);
void upload_sensor_data(const sensor_data_t* sensor_data);
