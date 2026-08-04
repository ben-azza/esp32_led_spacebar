# ESP32 LED Control via ROS2 / micro-ROS

Control an ESP32's onboard LED in real time by pressing the spacebar on a keyboard, using ROS2 (Jazzy) and micro-ROS over WiFi.

## Overview

This project connects a PC running ROS2 to a physical ESP32 board over WiFi using micro-ROS. Pressing **spacebar** on the PC publishes a `std_msgs/Bool` message on the `/led_cmd` topic, which the ESP32 subscribes to and uses to toggle its onboard LED (GPIO2).

## Architecture

```
[Keyboard input] --(ROS2 topic /led_cmd)--> [micro-ROS Agent] --(WiFi/UDP)--> [ESP32 firmware] --> [LED]
```

## Project structure

```
esp32_led_spacebar/
├── ros2_ws/                          # ROS2 workspace (PC side)
│   └── src/
│       ├── esp32_led_teleop/         # Python package (ament_python)
│       │   └── keyboard_led_publisher.py   # reads spacebar, publishes /led_cmd
│       └── esp32_led_status/         # C++ package (ament_cmake)
│           └── led_status_listener.cpp     # subscribes to /led_cmd, logs ON/OFF
└── esp32_firmware/                   # PlatformIO project (ESP32 side)
    ├── platformio.ini
    └── src/main.cpp                  # micro-ROS subscriber, drives GPIO2
```

## Requirements

- Ubuntu 24.04
- ROS2 Jazzy
- `micro_ros_agent` (built from source or installed)
- PlatformIO (`pip3 install --break-system-packages platformio`)
- ESP32-WROOM-32 dev board
- PC and ESP32 on the same WiFi network

## Setup

### 1. Clone the repository

```bash
git clone <YOUR_GITHUB_REPO_URL>
cd esp32_led_spacebar
```

### 2. Build the ROS2 workspace

```bash
cd ros2_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-select esp32_led_teleop esp32_led_status
source install/setup.bash
```

### 3. Configure and flash the ESP32 firmware

Edit `esp32_firmware/src/main.cpp` and set:
```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
const char* AGENT_IP  = "YOUR_PC_IP";   // find it with: hostname -I
```

Then build and upload:
```bash
cd ../esp32_firmware
pio run -t upload
```

## Running the project

**Terminal 1 — start the micro-ROS agent:**
```bash
ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888
```
Reset the ESP32 (or power-cycle it) and wait for the connection log.

**Terminal 2 — start the status listener (optional, for debugging):**
```bash
cd ros2_ws
source install/setup.bash
ros2 run esp32_led_status led_status_listener
```

**Terminal 3 — start the keyboard publisher:**
```bash
cd ros2_ws
source install/setup.bash
ros2 run esp32_led_teleop keyboard_led_publisher
```

Press **SPACE** in Terminal 3 → the ESP32's onboard LED toggles ON/OFF.  
Press **q** to quit the publisher.

## Troubleshooting

- **`Package not found` after `ros2 run`**: rebuild with `colcon build`, then re-source `install/setup.bash` in that terminal.
- **ESP32 not detected on USB**: check `ls /dev/ttyUSB*` and add your user to the `dialout` group (`sudo usermod -a -G dialout $USER`, then re-login).
- **ESP32 never connects to the agent**: double-check WiFi credentials and that `AGENT_IP` matches your PC's current IP (it can change on DHCP renewal).
- **IntelliSense errors on `std_msgs/msg/bool.h` in VSCode**: harmless before the first real build — run `pio run` once, then reload the VSCode window.

## Author

BEN___AZZA — INSAT, Informatique Industrielle et Automatique (IIA)
