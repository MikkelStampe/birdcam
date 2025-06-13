idf_component_register(SRCS "main.c" "camera.c" "sensors.c" "upload.c" "dht.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_http_client esp_driver_gpio esp_adc esp_timer)
