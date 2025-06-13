# How to Export the Flow Chart as PNG

## Method 1: Using Mermaid Live Editor

1. Go to [Mermaid Live Editor](https://mermaid.live/)
2. Copy the flow chart code from below and paste it into the editor
3. Click the "Download PNG" button in the top toolbar
4. Save the file as `ESP32_BirdCam_Flow_Chart.png`

## Method 2: Using VS Code with Mermaid Extension

1. Install the "Mermaid Markdown Syntax Highlighting" extension in VS Code
2. Create a new file with `.mmd` extension
3. Paste the flow chart code below
4. Right-click and select "Export to PNG"

## Method 3: Using Command Line (mermaid-cli)

```bash
npm install -g @mermaid-js/mermaid-cli
mmdc -i flowchart.mmd -o ESP32_BirdCam_Flow_Chart.png
```

## Flow Chart Code

```mermaid
flowchart TD
    Start([PIR Motion Detected]) --> Wake[ESP32 Wakes from Deep Sleep]
    Wake --> Init[Initialize System]
    Init --> CamInit[init_camera]
    
    CamInit --> Capture[capture_camera_sensor]
    Capture --> CaptureCheck{Image Captured?}
    CaptureCheck -->|Yes| Upload1[upload_image to /upload]
    CaptureCheck -->|No| Error1[Log Error]
    Error1 --> Sensors
    
    Upload1 --> UploadCheck1{Upload Success?}
    UploadCheck1 -->|Yes| Release[release_camera_sensor]
    UploadCheck1 -->|No| Error2[Log Upload Error]
    Error2 --> Release
    
    Release --> Sensors[collect_all_sensor_data]
    Sensors --> DHT[read_dht_data<br/>Temperature & Humidity]
    DHT --> Battery[read_battery_voltage<br/>Battery Status]
    Battery --> Validate[Validate Sensor Data]
    
    Validate --> Log[log_sensor_data<br/>Display Summary]
    Log --> CreateJSON[Create JSON Payload<br/>with Timestamp]
    CreateJSON --> Upload2[upload_sensor_data<br/>to /sensor-data]
    
    Upload2 --> UploadCheck2{Upload Success?}
    UploadCheck2 -->|Yes| Success[All Operations Complete]
    UploadCheck2 -->|No| Error3[Log Sensor Upload Error]
    Error3 --> Success
    
    Success --> Cleanup[System Cleanup]
    Cleanup --> Configure[Configure PIR Wake-up]
    Configure --> Sleep[Enter Deep Sleep]
    Sleep --> Start
    
    %% Data Flow Annotations
    Capture -.->|Frame Buffer| Upload1
    Sensors -.->|sensor_data_t struct| CreateJSON
    CreateJSON -.->|JSON String| Upload2
    
    %% Component Groups
    subgraph Camera[Camera Operations]
        CamInit
        Capture
        Release
    end
    
    subgraph SensorOps[Sensor Operations]
        Sensors
        DHT
        Battery
        Validate
        Log
    end
    
    subgraph Upload[Upload Operations]
        Upload1
        Upload2
        CreateJSON
    end
    
    subgraph Power[Power Management]
        Wake
        Configure
        Sleep
    end
    
    %% Styling
    style Start fill:#e1f5fe
    style Sleep fill:#e1f5fe
    style Camera fill:#f3e5f5
    style SensorOps fill:#e8f5e8
    style Upload fill:#fff3e0
    style Power fill:#fce4ec
    style Success fill:#e8f5e8
    style Error1 fill:#ffebee
    style Error2 fill:#ffebee
    style Error3 fill:#ffebee
```

---

Once exported as PNG, place the file in your project directory as `ESP32_BirdCam_Flow_Chart.png` to display it in the main documentation file. 