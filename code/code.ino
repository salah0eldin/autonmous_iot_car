// =========================================================
// NodeMCU Car Control
// This code is designed to control a NodeMCU-based car with steering and throttle control via a web interface.
// It uses an ESP32 with a web server to handle commands from a web page.
// The car can be steered left or right, and it can move forward or backward with throttle control.
// The code also includes a simple HTML interface for user interaction.
// =========================================================

// =========================================================
// Libraries
// =========================================================
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// =========================================================
// Constants and Definitions
// =========================================================
// Throttle values
#define DAC_VALUE_RUN   (255 * 1.8 / 3.3)
#define DAC_VALUE_STOP  (255 * 0.8 / 3.3)

// control pins
#define PIN_THOTTLE   25
#define PIN_BRAKES    27
#define PIN_CAR_DIR   26

// steering pins
#define PIN_STEER_DIR 33  // Stepper DIR
#define PIN_STEP      32  // Stepper STEP

// =========================================================
// Enums for car direction and brakes
// =========================================================
// forward normally opened
enum {
  backward = 0,
  forward
};

// brakes off normally opened
enum {
  brakes_on = 0,
  brakes_off
};

// steering
typedef enum {
  st_right = 0,
  st_left
} SteeringDir;

// =========================================================
// Global Variables
// =========================================================
// Const variables:
const int maxSteeringSteps = 100;   // Limit to prevent oversteering
const int braking_time = 100;       // ms

int currentPosition = 0;            // Track current step position
int Stepper_PUL_Width = 3000;       // Pulse width in micro sec
SteeringDir dir = st_right;

// WIFI variables:
String command = "S";               //String to store app command state.
const char* ssid = "Wifi Car";
const char* password = "12345678";
WebServer server(80);

// =========================================================
// HTML Page
// =========================================================
// HTML page
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>NodeMCU_Car</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body { font-family: Arial, sans-serif; text-align: center; background: #fff; margin: 0; }
    .header { background: #888; color: #fff; padding: 10px; font-size: 22px; text-align: left; }
    .ip { margin: 20px 0 10px 0; font-size: 16px; font-weight: bold; }
    .ip-value { font-size: 36px; color: #888; font-weight: bold; }
    .controls { display: grid; grid-template-columns: repeat(3, 100px); grid-template-rows: repeat(3, 100px); gap: 10px; justify-content: center; margin: 40px 0; }
    .btn { background: none; border: none; outline: none; cursor: pointer; }
    .btn svg { width: 60px; height: 60px; }
    .slider-container { margin: 40px 0; display: flex; flex-direction: column; align-items: center; gap: 30px; }
    .slider-label { font-size: 18px; font-weight: bold; margin-bottom: 8px; color: #444; }
    .slider-value { font-size: 16px; color: #888; margin-left: 10px; }
    .slider { width: 80%; }
    .toggle-row { display: flex; flex-direction: row; align-items: center; justify-content: center; gap: 10px; margin-top: 20px; }
    .toggle-btn, .home-btn {
      background: #6cf;
      color: #fff;
      border: none;
      border-radius: 20px;
      padding: 7px 18px;
      font-size: 14px;
      cursor: pointer;
      transition: background 0.2s;
      margin: 0;
    }
    .toggle-btn.active {
      background: #888;
    }
    .home-btn { background: #888; }
    @media (max-width: 600px) {
      .slider-label, .slider-value { font-size: 14px; }
      .toggle-btn, .home-btn { font-size: 12px; padding: 6px 12px; }
    }
  </style>
</head>
<body>
  <div class="header">NodeMCU_Car</div>
  <div class="controls">
    <!-- Top left (should be bottom right) -->
    <button class="btn" onmousedown="sendCmd('J')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('J')" ontouchend="sendCmd('S')">
      <svg><polygon points="30,50 50,30 30,30" fill="#888"/><polygon points="30,50 40,30 30,30" fill="#6cf"/></svg>
    </button>
    <!-- Up -->
    <button class="btn" onmousedown="sendCmd('F')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('F')" ontouchend="sendCmd('S')">
      <svg><polygon points="30,10 50,30 10,30" fill="#888"/><polygon points="30,10 40,30 20,30" fill="#6cf"/></svg>
    </button>
    <!-- Top right (should be bottom left) -->
    <button class="btn" onmousedown="sendCmd('H')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('H')" ontouchend="sendCmd('S')">
      <svg><polygon points="30,50 10,30 30,30" fill="#888"/><polygon points="30,50 20,30 30,30" fill="#6cf"/></svg>
    </button>
    <!-- Left -->
    <button class="btn" onmousedown="sendCmd('L')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('L')" ontouchend="sendCmd('S')">
      <svg><polygon points="10,30 30,50 30,10" fill="#888"/><polygon points="10,30 30,40 30,20" fill="#6cf"/></svg>
    </button>
    <!-- Stop -->
    <button class="btn" onmousedown="sendCmd('S')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('S')" ontouchend="sendCmd('S')">
      <svg><circle cx="30" cy="30" r="25" stroke="#888" stroke-width="5" fill="none"/><rect x="20" y="20" width="20" height="20" fill="#6cf"/></svg>
    </button>
    <!-- Right -->
    <button class="btn" onmousedown="sendCmd('R')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('R')" ontouchend="sendCmd('S')">
      <svg><polygon points="50,30 30,50 30,10" fill="#888"/><polygon points="50,30 30,40 30,20" fill="#6cf"/></svg>
    </button>
    <!-- Bottom left (should be top right) -->
    <button class="btn" onmousedown="sendCmd('I')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('I')" ontouchend="sendCmd('S')">
      <svg><polygon points="30,10 50,30 30,30" fill="#888"/><polygon points="30,10 40,30 30,30" fill="#6cf"/></svg>
    </button>
    <!-- Down -->
    <button class="btn" onmousedown="sendCmd('B')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('B')" ontouchend="sendCmd('S')">
      <svg><polygon points="30,50 50,30 10,30" fill="#888"/><polygon points="30,50 40,30 20,30" fill="#6cf"/></svg>
    </button>
    <!-- Bottom right (should be top left) -->
    <button class="btn" onmousedown="sendCmd('G')" onmouseup="sendCmd('S')" ontouchstart="sendCmd('G')" ontouchend="sendCmd('S')">
      <svg><polygon points="30,10 10,30 30,30" fill="#888"/><polygon points="30,10 20,30 30,30" fill="#6cf"/></svg>
    </button>
  </div>
  <div class="slider-container">
    <div>
      <span class="slider-label">Car Speed</span>
      <input type="range" min="0" max="100" value="50" class="slider" id="speedSlider" oninput="updateSpeedValue(this.value); sendSpeed(this.value)">
      <span class="slider-value" id="speedValue">50</span>
    </div>
    <div>
      <span class="slider-label">Steering Speed</span>
      <input type="range" min="0" max="100" value="50" class="slider" id="steerSlider" oninput="updateSteerValue(this.value); sendSteerSpeed(this.value)">
      <span class="slider-value" id="steerValue">50</span>
    </div>
    <div class="toggle-row">
      <button id="autoHomeBtn" class="toggle-btn" onclick="toggleAutoHome()">Enable Auto Go Home</button>
      <button id="manualHomeBtn" class="home-btn" onclick="sendGoHome()">Go Home</button>
    </div>
  </div>
  <script>
    function sendCmd(cmd) {
      fetch('/cmd?dir=' + cmd);
    }
    function sendSpeed(val) {
      fetch('/speed?val=' + val);
    }
    function sendSteerSpeed(val) {
      fetch('/speed?steer=' + val);
    }
    function updateSpeedValue(val) {
      document.getElementById('speedValue').textContent = val;
    }
    function updateSteerValue(val) {
      document.getElementById('steerValue').textContent = val;
    }
    function toggleAutoHome() {
      var btn = document.getElementById('autoHomeBtn');
      var manualBtn = document.getElementById('manualHomeBtn');
      var enabled = btn.classList.toggle('active');
      btn.textContent = enabled ? 'Disable Auto Go Home' : 'Enable Auto Go Home';
      manualBtn.style.display = enabled ? 'none' : 'inline-block';
      fetch('/cmd?autoHome=' + (enabled ? '1' : '0'));
    }
    function sendGoHome() {
      var btn = document.getElementById('manualHomeBtn');
      fetch('/cmd?manualHome=' + '1');
    }
    window.onload = function() {
      var btn = document.getElementById('autoHomeBtn');
      var manualBtn = document.getElementById('manualHomeBtn');
      if (!btn.classList.contains('active')) {
        manualBtn.style.display = 'inline-block';
      }
    }
  </script>
</body>
</html>
)rawliteral";

// =========================================================
// Forward declarations for functions used before definition
// =========================================================
void goHome();
void goAhead();
void goBack();
void goLeft();
void goRight();
void stopRobot();
void setSpeed(int speed);
void setSteer(int speed);
void moveDirection(String dir);

// =========================================================
// Setup Function
// =========================================================
void setup() {
  pinMode(PIN_THOTTLE, OUTPUT);
  pinMode(PIN_BRAKES, OUTPUT);
  pinMode(PIN_CAR_DIR, OUTPUT);
  pinMode(PIN_STEER_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);

  Serial.begin(115200);

  // Connecting WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Serve the main page
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  // Handle direction commands
  server.on("/cmd", []() {
    if (server.hasArg("dir")) {
      String dir = server.arg("dir");
      Serial.print("Received command: ");
      Serial.println(dir); // <-- Print the command sent
      moveDirection(dir);
      server.send(200, "text/plain", "OK");
    } else if (server.hasArg("autoHome")) {
      String autoHome = server.arg("autoHome");
      if (autoHome == "1") {
        Serial.println("Auto Go Home enabled");
        // Here you can implement the logic for auto home
        goHome();
      } else if (server.hasArg("manualHome")) {
        Serial.println("Manual Go Home triggered");
        goHome();
      }
      else {
        Serial.println("Auto Go Home disabled");
      }
      server.send(200, "text/plain", "OK");
    } 
    else {
      server.send(400, "text/plain", "Missing dir");
    }
  });

  // Handle speed commands
  server.on("/speed", []() {
    if (server.hasArg("val")) {
      int speed = server.arg("val").toInt();
      Serial.print("Received command: ");
      Serial.println(speed); // <-- Print the command sent
      setSpeed(speed);
      server.send(200, "text/plain", "OK");
    } else if (server.hasArg("steer")) {
      int speed = server.arg("steer").toInt();
      Serial.print("Received command: ");
      Serial.println(speed); // <-- Print the command sent
      setSteer(speed);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing val");
    }
  });

  server.begin();
}

// =========================================================
// Loop Function
// =========================================================
void loop() {
  server.handleClient();
  static String lastCommand = "";
  String newCommand = server.arg("State");

  if (newCommand != "" && newCommand != lastCommand) {
    command = newCommand;
    Serial.print("CMD = ");
    Serial.println(command);
    switch (command[0]) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        {
          Stepper_PUL_Width = (command[0] - '0' + 1) * 1000;
          break;
        }
      case 'F':
        {
          goHome();
          goAhead();
          break;
        }
      case 'B':
        {
          goHome();
          goBack();
          break;
        }
      case 'S':
        {
          stopRobot();
          break;
        }

      case 'L':
        {
          dir = st_left;
          while (server.arg("State") == "L") {
            goLeft();
            server.handleClient();
          }
          break;
        }
      case 'R':
        {
          dir = st_right;
          while (server.arg("State") == "R") {
            goRight();
            server.handleClient();
          }
          break;
        }
      case 'I':
        {
          goAhead();
          dir = st_right;
          while (server.arg("State") == "I") {
            goRight();
            server.handleClient();
          }
          break;
        }
      case 'G':
        {
          goAhead();
          dir = st_left;
          while (server.arg("State") == "G") {
            goLeft();
            server.handleClient();
          }
          break;
        }
      case 'J':
        {
          goBack();
          dir = st_right;
          while (server.arg("State") == "J") {
            goRight();
            server.handleClient();
          }
          break;
        }
      case 'H':
        {
          goBack();
          dir = st_left;
          while (server.arg("State") == "H") {
            goLeft();
            server.handleClient();
          }
          break;
        }
    }
    lastCommand = command;
  }
}

// =========================================================
// Core Motor Functions
// =========================================================
void stepMotor(SteeringDir dir) {
  //Set direction
  digitalWrite(PIN_STEER_DIR, dir);
  //move one step
  digitalWrite(PIN_STEP, HIGH);
  delayMicroseconds(Stepper_PUL_Width);
  digitalWrite(PIN_STEP, LOW);
  delayMicroseconds(Stepper_PUL_Width);
}

void goAhead() {
  //Release brakes
  digitalWrite(PIN_BRAKES, brakes_off);
  //Set direction
  digitalWrite(PIN_CAR_DIR, forward);
  //apply throttle
  dacWrite(PIN_THOTTLE, DAC_VALUE_RUN);
}

void goBack() {
  //Release brakes
  digitalWrite(PIN_BRAKES, brakes_off);
  //Set direction
  digitalWrite(PIN_CAR_DIR, backward);
  //apply throttle
  dacWrite(PIN_THOTTLE, DAC_VALUE_RUN);
}

void goLeft() {
  if (currentPosition < maxSteeringSteps) {
    stepMotor(st_left);
    currentPosition++;
  }
}

void goRight() {
  if (currentPosition < maxSteeringSteps) {
    stepMotor(st_right);
    currentPosition++;
  }
}

void goHome() {
  //Check if Already at Home
  if (currentPosition == 0) {
    Serial.println("Already at Home");
    return;
  }
  //Reverse direction
  dir = ((dir == st_right) ? st_left : st_right);
  digitalWrite(PIN_STEER_DIR, dir);
  //Move back the current position
  for (int i = 0; i < currentPosition; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(Stepper_PUL_Width);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(Stepper_PUL_Width);
  }
  currentPosition = 0;
  Serial.println("Steering centered");
}

void stopRobot() {
  // Steering home
  goHome();
  // Release  throttle
  dacWrite(PIN_THOTTLE, DAC_VALUE_STOP);
  // Activate brakes
  digitalWrite(PIN_BRAKES, brakes_on);
  delay(braking_time);
  digitalWrite(PIN_BRAKES, brakes_off);
}

void moveDirection(String dir) {
  if (dir == "up") {
    goAhead();
  } else if (dir == "down") {
    goBack();
  } else if (dir == "left") {
    goLeft();
  } else if (dir == "right") {
    goRight();
  } else if (dir == "stop") {
    stopRobot();
  }
  // Add more as needed for up_left, up_right, etc.
}

void setSpeed(int speed) {
  // Clamp speed to 0-255
  speed = constrain(speed, 0, 255);
  dacWrite(PIN_THOTTLE, speed);
}

void setSteer(int speed) {
  // Clamp speed to 0-255
  speed = constrain(speed, 0, 255);
  dacWrite(PIN_THOTTLE, speed);
}

void HTTP_handleRoot(void) {
  server.send(200, "text/html", "");
  delay(1);
}
