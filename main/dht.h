
#pragma once
#include "esp_err.h"

#define DHT_TYPE_DHT22 22
esp_err_t dht_read_data(int type, gpio_num_t pin, int16_t *humidity, int16_t *temperature);
