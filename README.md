# NodeMCU Car Control System

An ESP32-based car control system with web interface for steering and throttle control.

## 🚗 Overview

This project implements a remote control system for a NodeMCU-based car using an ESP32 microcontroller. The system provides web-based control through a responsive interface accessible from any device with a web browser.

### Key Features

- **Web Control**: HTML interface for car control
- **Position Memory**: EEPROM-based steering position storage
- **Directional Control**: 8-direction steering with throttle control
- **Auto Home Function**: Automatic steering return to center
- **Speed Control**: Independent car and steering speed adjustment
- **Touch Support**: Designed for mobile and touch devices

## 🔧 Hardware Requirements

### Required Components

| Component | Quantity | Description |
|-----------|----------|-------------|
| ESP32 Development Board | 1 | Main microcontroller |
| Stepper Motor | 1 | For steering control |
| DC Motor | 1 | For car propulsion |
| Motor Driver | 1 | L298N or similar |
| Car Chassis | 1 | With wheels and mounting |
| Power Supply | 1 | 12V for motors, 3.3V for ESP32 |
| Jumper Wires | As needed | For connections |

## 🔌 Pin Configuration

### Motor Control Pins

| Pin | Function | Description |
|-----|----------|-------------|
| GPIO 25 | `PIN_THOTTLE` | DAC output for throttle control |
| GPIO 27 | `PIN_BRAKES` | Brake control (normally open) |
| GPIO 26 | `PIN_CAR_DIR` | Car direction control |

### Steering Control Pins

| Pin | Function | Description |
|-----|----------|-------------|
| GPIO 33 | `PIN_STEER_DIR` | Stepper motor direction |
| GPIO 32 | `PIN_STEP` | Stepper motor step signal |

## 📦 Installation

### Prerequisites

- Arduino IDE 1.8.x or later
- ESP32 board support package

### Setup Instructions

1. **Install ESP32 Board Support**
   - Open Arduino IDE
   - Go to `File > Preferences`
   - Add ESP32 board manager URL:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to `Tools > Board > Boards Manager`
   - Search for "ESP32" and install

2. **Upload the Code**
   - Open `code/code.ino` in Arduino IDE
   - Select your ESP32 board
   - Click Upload

## 🎮 Usage

### Initial Setup

1. **Power On**: Connect power supply and turn on the car
2. **WiFi Connection**: The ESP32 creates an access point
   - SSID: `Wifi Car`
   - Password: `12345678`
3. **Web Interface**: Connect to the WiFi and navigate to `192.168.4.1`

### Control Interface

#### Direction Controls

| Button | Function | Description |
|--------|----------|-------------|
| **F** | Forward | Move car forward |
| **B** | Backward | Move car backward |
| **L** | Left | Turn steering left |
| **R** | Right | Turn steering right |
| **I** | Forward + Right | Forward with right turn |
| **G** | Forward + Left | Forward with left turn |
| **J** | Backward + Right | Backward with right turn |
| **H** | Backward + Left | Backward with left turn |
| **S** | Stop | Stop all movement |

#### Speed Controls

- **Car Speed**: 0-100% throttle control
- **Steering Speed**: 1-100% stepper motor speed

#### Advanced Features

- **Auto Go Home**: Automatically return steering to center when stopping
- **Manual Go Home**: Manually return steering to center
- **Position Memory**: Steering position persists across power cycles

## 🔌 API Reference

### HTTP Endpoints

#### GET `/`
Serves the main web interface.

**Response**: HTML page

#### POST `/cmd`
Handles direction and home commands.

**Parameters**:
- `dir`: Direction command (F, B, L, R, I, G, J, H, S)
- `autoHome`: Enable/disable auto home (0 or 1)
- `manualHome`: Trigger manual home (1)

**Response**: `text/plain` "OK" on success

#### POST `/speed`
Handles speed control commands.

**Parameters**:
- `car`: Car speed (0-100)
- `steer`: Steering speed (1-100)

**Response**: `text/plain` "OK" on success

### Configuration Constants

```cpp
// Steering limits
const int max_steering_steps = 100;  // Maximum steering steps

// Timing
const int braking_time = 100;        // Brake activation time (ms)

// EEPROM configuration
#define EEPROM_SIZE 512
#define EEPROM_INDICATOR_ADDR 0
#define EEPROM_POSITION_ADDR 1
#define EEPROM_INDICATOR_VALUE 0xA0

// WiFi configuration
const char* ssid = "Wifi Car";
const char* password = "12345678";
```

## ⚙️ Configuration

### WiFi Settings

To change WiFi credentials, modify these lines in `code.ino`:

```cpp
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";
```

### Steering Configuration

Adjust steering sensitivity and limits:

```cpp
const int max_steering_steps = 100;  // Increase for wider turns
int stepper_pulse_width = 3000;      // Decrease for faster steering
```

## 🔧 Troubleshooting

### Common Issues

#### Car Not Responding
1. **Check Power Supply**: Ensure adequate power for motors
2. **Verify Connections**: Check all pin connections
3. **Serial Monitor**: Check for error messages at 115200 baud

#### WiFi Connection Issues
1. **Reset ESP32**: Power cycle the device
2. **Check SSID**: Ensure connecting to "Wifi Car"
3. **Password**: Use "12345678" as password

#### Steering Problems
1. **Check Stepper Wiring**: Verify DIR and STEP connections
2. **Power Supply**: Ensure stepper motor has adequate power
3. **Position Reset**: Use "Go Home" button to reset position

### Debug Information

Enable Serial Monitor to view debug information:

```
AP IP address: 192.168.4.1
Web request received for /
Received command: F
Position changed: 5
Restored position from EEPROM: 5
```

## 📄 License

This project is licensed under the MIT License.
