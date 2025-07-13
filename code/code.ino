// =========================================================
// NodeMCU Car Control

// This code is designed to control a NodeMCU-based car
// with steering and throttle control via a web interface.

// It uses an ESP32 with a web server to handle
// commands from a web page.

// The car can be steered left or right, and it can move
// forward or backward with throttle control.

// The code also includes a simple HTML interface
// for user interaction.
// =========================================================

// =========================================================
// Libraries
// =========================================================
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "webpage.h"

// =========================================================
// Constants and Definitions
// =========================================================
// Throttle stop/min value
#define DAC_MIN_VALUE (255 * 0.8 / 3.3)

// control pins
#define PIN_THOTTLE 25
#define PIN_BRAKES 27
#define PIN_CAR_DIR 26

// steering pins
#define PIN_STEER_DIR 33  // Stepper DIR
#define PIN_STEP 32       // Stepper STEP

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
enum {
  st_right = 0,
  st_left
};

// =========================================================
// Global Variables
// =========================================================
// Const variables:
const int max_steering_steps = 100;  // Limit to prevent oversteering
const int braking_time = 100;        // ms

int car_speed = DAC_MIN_VALUE;
int current_position = 0;        // Track current step position
int stepper_pulse_width = 3000;  // Pulse width in micro sec
bool auto_home_flag = 0;

// WIFI variables:
String command = "S";  //String to store app command state.
const char* ssid = "Wifi Car";
const char* password = "12345678";
WebServer server(80);

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
  server.on("/", HTTP_GET, []() {
    Serial.println("Web request received for /");
    server.send(200, "text/html", htmlPage);
  });

  // Handle direction commands
  server.on("/cmd", []() {
    if (server.hasArg("dir")) {

      String dir = server.arg("dir");

      Serial.print("Received command: ");
      Serial.println(dir);  // <-- Print the command sent

      moveDirection(dir);

      server.send(200, "text/plain", "OK");

    } else if (server.hasArg("autoHome")) {

      String autoHome = server.arg("autoHome");

      if (autoHome == "1") {

        Serial.println("Auto Go Home enabled");

        goHome();

      } else {

        Serial.println("Auto Go Home disabled");
      }

      server.send(200, "text/plain", "OK");

    } else if (server.hasArg("manualHome")) {

      Serial.println("Manual Go Home triggered");

      goHome();

      server.send(200, "text/plain", "OK");

    } else {

      server.send(400, "text/plain", "Missing dir");
    }
  });

  // Handle speed commands
  server.on("/speed", []() {
    if (server.hasArg("car")) {

      int speed = server.arg("car").toInt();

      Serial.print("Received command: ");
      Serial.println(speed);  // <-- Print the command sent

      setSpeed(speed);

      server.send(200, "text/plain", "OK");

    } else if (server.hasArg("steer")) {

      int speed = server.arg("steer").toInt();

      Serial.print("Received command: ");
      Serial.println(speed);  // <-- Print the command sent

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
}

// =========================================================
// Core Motor Functions
// =========================================================
void stepMotor() {
  //move one step
  digitalWrite(PIN_STEP, HIGH);
  delayMicroseconds(stepper_pulse_width);
  digitalWrite(PIN_STEP, LOW);
  delayMicroseconds(stepper_pulse_width);
}

void setDirection(SteeringDir dir) {
  //Set direction
  digitalWrite(PIN_STEER_DIR, dir);
}

void goAhead() {
  //Release brakes
  digitalWrite(PIN_BRAKES, brakes_off);
  //Set direction
  digitalWrite(PIN_CAR_DIR, forward);
  //apply throttle
  dacWrite(PIN_THOTTLE, car_speed);
}

void goBack() {
  //Release brakes
  digitalWrite(PIN_BRAKES, brakes_off);
  //Set direction
  digitalWrite(PIN_CAR_DIR, backward);
  //apply throttle
  dacWrite(PIN_THOTTLE, car_speed);
}

void goLeft() {
  if (current_position < max_steering_steps) {
    stepMotor();
    current_position++;
  }
}

void goRight() {
  if (abs(current_position) <= max_steering_steps) {
    stepMotor();
    current_position--;
  }
}

void goHome() {
  //Check if Already at Home
  if (current_position == 0) {
    Serial.println("Already at Home");
    return;
  }

  // Set direction
  setDirection(current_position > 0 ? st_left : st_right);

  // Move back Home
  while (current_position != 0) {
    stepMotor();
    current_position += (current_position > 0 ? -1 : 1);
  }

  Serial.println("Steering Reached Home");
}

void stopRobot() {
  // Release  throttle
  dacWrite(PIN_THOTTLE, DAC_MIN_VALUE);

  // Activate brakes
  digitalWrite(PIN_BRAKES, brakes_on);

  // Steering home
  if (auto_home_flag) {
    goHome();
  }
}

void moveDirection(String dir) {
  switch (dir[0]) {
    case 'F':
      {
        goAhead();
        break;
      }
    case 'B':
      {
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
        setDirection(st_left);
        while (server.arg("State") == "L") {
          goLeft();
          server.handleClient();
        }
        break;
      }
    case 'R':
      {
        setDirection(st_right);
        while (server.arg("State") == "R") {
          goRight();
          server.handleClient();
        }
        break;
      }
    case 'I':
      {
        goAhead();
        setDirection(st_right);
        while (server.arg("State") == "I") {
          goRight();
          server.handleClient();
        }
        break;
      }
    case 'G':
      {
        goAhead();
        setDirection(st_left);
        while (server.arg("State") == "G") {
          goLeft();
          server.handleClient();
        }
        break;
      }
    case 'J':
      {
        goBack();
        setDirection(st_right);
        while (server.arg("State") == "J") {
          goRight();
          server.handleClient();
        }
        break;
      }
    case 'H':
      {
        goBack();
        setDirection(st_left);
        while (server.arg("State") == "H") {
          goLeft();
          server.handleClient();
        }
        break;
      }
  }
}

void setSpeed(int speed) {
  // map speed to min-255
  car_speed = map(speed, 0, 100, DAC_MIN_VALUE, 255);
}

void setSteer(int speed) {
  // map speed to min-255
  stepper_pulse_width = map(speed, 1, 100, 10000, 1000);
}
