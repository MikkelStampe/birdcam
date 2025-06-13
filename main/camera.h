#pragma once

#include "esp_camera.h"

void init_camera();
camera_fb_t* capture_image();
void release_image(camera_fb_t* fb);
