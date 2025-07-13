# NodeMCU Car Control System

A sophisticated IoT-based car control system built with ESP32, featuring real-time web interface control, persistent position memory, and advanced steering/throttle management.

## 🚗 Overview

This project implements a complete remote control system for a NodeMCU-based car using an ESP32 microcontroller. The system provides intuitive web-based control through a modern, responsive interface accessible from any device with a web browser.

### Key Features

- **Real-time Web Control**: Modern, responsive web interface
- **Persistent Position Memory**: EEPROM-based steering position storage
- **Multi-directional Control**: 8-direction steering with throttle control
- **Auto Home Function**: Automatic steering return to center
- **Speed Control**: Independent car and steering speed adjustment
- **Touch Optimized**: Designed for mobile and touch devices
- **Professional UI**: Glass morphism design with smooth animations

## 📋 Table of Contents

- [Hardware Requirements](#hardware-requirements)
- [Pin Configuration](#pin-configuration)
- [Installation](#installation)
- [Usage](#usage)
- [API Reference](#api-reference)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

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

### Optional Components

- **Battery Pack**: For mobile operation
- **Voltage Regulator**: For stable power supply
- **LED Indicators**: For status indication
- **Buzzer**: For audio feedback

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

### Power Configuration

- **Motor Power**: 12V supply
- **Logic Power**: 3.3V from ESP32
- **DAC Output**: 0-3.3V for throttle control

## 📦 Installation

### Prerequisites

- Arduino IDE 1.8.x or later
- ESP32 board support package
- Required libraries (included in code)

### Setup Instructions

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/nodemcu-car-control.git
   cd nodemcu-car-control
   ```

2. **Install ESP32 Board Support**
   - Open Arduino IDE
   - Go to `File > Preferences`
   - Add ESP32 board manager URL:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to `Tools > Board > Boards Manager`
   - Search for "ESP32" and install

3. **Configure Board Settings**
   - Board: `ESP32 Dev Module`
   - Upload Speed: `115200`
   - CPU Frequency: `240MHz`
   - Flash Frequency: `80MHz`
   - Flash Mode: `QIO`
   - Flash Size: `4MB`

4. **Upload the Code**
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

### Web Interface Features

- **Responsive Design**: Works on desktop, tablet, and mobile
- **Touch Optimized**: Designed for touch screen interaction
- **Real-time Updates**: Live position and status feedback
- **Modern UI**: Glass morphism design with smooth animations

## 🔌 API Reference

### HTTP Endpoints

#### GET `/`
Serves the main web interface.

**Response**: HTML page

#### GET `/position`
Returns current steering position.

**Response**: `text/plain` with position value (-100 to +100)

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

### Speed Configuration

Modify speed mapping:

```cpp
// In setSpeed() function
car_speed = map(speed, 0, 100, DAC_MIN_VALUE, 255);

// In setSteer() function
stepper_pulse_width = map(speed, 1, 100, 10000, 1000);
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

#### Web Interface Issues
1. **Browser Cache**: Clear browser cache and refresh
2. **Device Compatibility**: Try different browser or device
3. **Network Issues**: Ensure device is connected to car's WiFi

### Debug Information

Enable Serial Monitor to view debug information:

```
AP IP address: 192.168.4.1
Web request received for /
Received command: F
Position changed: 5
Restored position from EEPROM: 5
```

### Performance Optimization

- **Steering Speed**: Adjust `stepper_pulse_width` for optimal performance
- **Update Frequency**: Modify position update interval in web interface
- **Memory Usage**: Monitor EEPROM usage for position storage

## 🤝 Contributing

We welcome contributions! Please follow these guidelines:

1. **Fork the Repository**: Create your own fork
2. **Create Feature Branch**: `git checkout -b feature/amazing-feature`
3. **Commit Changes**: `git commit -m 'Add amazing feature'`
4. **Push to Branch**: `git push origin feature/amazing-feature`
5. **Open Pull Request**: Submit your changes for review

### Development Setup

1. **Install Dependencies**: Ensure all required libraries are installed
2. **Follow Code Style**: Use consistent naming conventions
3. **Test Thoroughly**: Test on actual hardware before submitting
4. **Document Changes**: Update documentation for new features

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **ESP32 Community**: For excellent documentation and support
- **Arduino Community**: For the foundation of embedded development
- **Web Development Community**: For modern UI/UX practices

## 📞 Support

For support and questions:

- **Issues**: Create an issue on GitHub
- **Discussions**: Use GitHub Discussions
- **Email**: [your-email@example.com]

## 📈 Roadmap

### Planned Features

- [ ] **Bluetooth Control**: Add Bluetooth connectivity option
- [ ] **GPS Navigation**: Autonomous navigation capabilities
- [ ] **Camera Integration**: Video streaming from car
- [ ] **Mobile App**: Native mobile application
- [ ] **Multi-car Support**: Control multiple cars simultaneously
- [ ] **Advanced Sensors**: Obstacle detection and avoidance

### Version History

- **v1.0.0**: Initial release with basic web control
- **v1.1.0**: Added EEPROM position memory
- **v1.2.0**: Improved UI and touch support
- **v1.3.0**: Added auto home functionality

---

**Made with ❤️ for the IoT and Robotics Community**
