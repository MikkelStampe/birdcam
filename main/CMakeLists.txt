idf_component_register(SRCS "wifi_manager.c" "main.c" "camera.c" "sensors.c" "upload.c" "dht.c"
                    INCLUDE_DIRS "."
                    REQUIRES nvs_flash esp_wifi esp_http_client esp_driver_gpio esp_adc esp_timer)
