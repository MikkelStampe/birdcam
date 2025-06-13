# ESP32 BirdCam Architecture & Data Flow

## Overview

The ESP32 BirdCam is a motion-triggered wildlife monitoring system that captures images and environmental data, then uploads both to a remote server. The system is designed with a modular architecture separating camera operations, sensor management, and data upload functionality.

## System Architecture

### Core Components

#### 1. **Camera Driver** (`camera.c/.h`)
- **Purpose**: Hardware abstraction for ESP32-CAM module
- **Responsibilities**:
  - Camera initialization with optimal settings
  - Image capture operations
  - Frame buffer management
- **Key Functions**:
  - `init_camera()`: Configures camera hardware
  - `capture_image()`: Captures image and returns frame buffer
  - `release_image()`: Properly releases frame buffer memory

#### 2. **Sensor Manager** (`sensors.c/.h`)
- **Purpose**: Unified interface for all sensor operations including camera
- **Responsibilities**:
  - Environmental data collection (DHT22)
  - Battery voltage monitoring
  - Camera sensor integration
  - Data aggregation and validation
- **Key Functions**:
  - `read_dht_data()`: Temperature and humidity readings
  - `read_battery_voltage()`: Battery status monitoring
  - `capture_camera_sensor()`: Camera as sensor interface
  - `collect_all_sensor_data()`: Unified data collection
  - `log_sensor_data()`: Formatted data logging

#### 3. **Upload Manager** (`upload.c/.h`)
- **Purpose**: Network communication and data transmission
- **Responsibilities**:
  - HTTP client management
  - Image upload (binary data)
  - Sensor data upload (JSON format)
  - Error handling and retry logic
- **Key Functions**:
  - `upload_image()`: Uploads binary image data
  - `upload_sensor_data()`: Uploads JSON sensor data

#### 4. **Main Controller** (`main.c`)
- **Purpose**: Application orchestration and power management
- **Responsibilities**:
  - System initialization
  - Workflow coordination
  - Deep sleep management
  - PIR motion detection handling

## Data Flow Architecture

### System Flow Chart

![ESP32 BirdCam Flow Chart](ESP32_BirdCam_Flow_Chart.png)

*The complete system flow chart is displayed above, showing the detailed workflow from PIR trigger to deep sleep.*

### Primary Workflow

```
PIR Motion Trigger → Wake from Deep Sleep → Initialize Camera → 
Capture Image → Upload Image → Collect Sensor Data → 
Upload Sensor Data → Enter Deep Sleep
```

### Detailed Flow Steps

1. **System Wake-up**
   - PIR sensor detects motion
   - ESP32 wakes from deep sleep
   - System initialization begins

2. **Camera Operations**
   - Camera hardware initialization
   - Image capture via sensor interface
   - Frame buffer management

3. **Image Processing & Upload**
   - Binary image data upload to `/upload` endpoint
   - Proper memory cleanup
   - Upload status verification

4. **Environmental Data Collection**
   - DHT22 sensor reading (temperature/humidity)
   - Battery voltage measurement via ADC
   - Data validation and error handling

5. **Sensor Data Upload**
   - JSON payload creation with timestamp
   - Upload to `/sensor-data` endpoint
   - Comprehensive logging

6. **Power Management**
   - System cleanup
   - Deep sleep activation
   - PIR interrupt configuration

## Data Structures

### Sensor Data Structure
```c
typedef struct {
    float temperature;      // in Celsius
    float humidity;        // in percentage  
    int battery_voltage;   // in mV
    bool dht_valid;       // DHT reading validity
    bool battery_valid;   // Battery reading validity
} sensor_data_t;
```

## Communication Protocols

### Image Upload
- **Endpoint**: `http://your-server.local/upload`
- **Method**: HTTP POST
- **Content-Type**: `application/octet-stream`
- **Format**: Binary JPEG data

### Sensor Data Upload
- **Endpoint**: `http://your-server.local/sensor-data`
- **Method**: HTTP POST
- **Content-Type**: `application/json`
- **Format**: JSON payload with sensor readings and metadata

### Sample JSON Payload
```json
{
  "temperature": 23.5,
  "humidity": 65.0,
  "battery_voltage": 3950,
  "dht_valid": true,
  "battery_valid": true,
  "timestamp": 1703123456
}
```

## Hardware Configuration

### Pin Assignments
- **Camera Pins**: Standard ESP32-CAM configuration
- **DHT22**: GPIO 13
- **PIR Sensor**: GPIO 12
- **Battery Monitor**: ADC1_CHANNEL_6

### Power Management
- Deep sleep between activations
- PIR sensor wake-up trigger
- Battery voltage monitoring for low-power alerts

## Key Features

### 🏗️ **Modular Architecture**
- Clear separation of concerns
- Independent component testing
- Easy maintenance and updates

### 📊 **Comprehensive Data Collection**
- Visual data (images)
- Environmental data (temperature, humidity)
- System health data (battery status)

### 🔄 **Robust Error Handling**
- Sensor validation flags
- Upload retry mechanisms
- Graceful failure recovery

### ⚡ **Power Efficiency**
- Deep sleep between events
- Optimized wake-up cycles
- Battery monitoring

### 📡 **Dual Upload System**
- Binary image uploads
- Structured sensor data
- Timestamped records

## System Benefits

1. **Reliability**: Modular design with error handling
2. **Extensibility**: Easy to add new sensors or modify behavior
3. **Efficiency**: Optimized power consumption
4. **Maintainability**: Clear code organization and documentation
5. **Data Integrity**: Validation and timestamping

## Future Enhancements

- Multiple image formats support
- Local storage backup
- Wireless configuration interface
- Advanced motion detection algorithms
- Cloud integration options

---

*This architecture provides a robust foundation for wildlife monitoring with comprehensive data collection and reliable transmission capabilities.* 