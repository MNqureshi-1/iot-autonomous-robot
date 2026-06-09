# 🚗 ESP32 Smart RC Car

An ESP32-powered smart RC car controlled through the Blynk IoT platform. The car supports manual remote control, autonomous obstacle avoidance, and light-following behavior using onboard sensors.

---

## 📋 Features

### 🎮 Manual Control

Control the car remotely using the Blynk mobile application.

* Forward
* Backward
* Left
* Right
* Variable speed control through firmware settings

### 🚧 Obstacle Avoidance Mode

Using an HC-SR04 ultrasonic sensor, the car can autonomously avoid obstacles.

Behavior:

1. Detect obstacle
2. Stop
3. Reverse
4. Turn right
5. Continue driving

### 💡 Light Following Mode

Using two LDR (Light Dependent Resistor) sensors, the car can detect and follow a light source.

Behavior:

* Compares light intensity on both sides
* Steers toward the brighter side
* Stops when both sensors detect darkness

---

## 🛠 Hardware Used

| Component                 | Quantity |
| ------------------------- | -------- |
| ESP32 Development Board   | 1        |
| TB6612FNG Motor Driver    | 1        |
| DC Gear Motors            | 2        |
| HC-SR04 Ultrasonic Sensor | 1        |
| LDR Sensors               | 2        |
| Robot Chassis             | 1        |
| Battery Pack              | 1        |

---

## 🔌 Pin Configuration

### Motor Driver

| ESP32 Pin | Function |
| --------- | -------- |
| GPIO 18   | AIN1     |
| GPIO 19   | AIN2     |
| GPIO 21   | BIN1     |
| GPIO 22   | BIN2     |
| GPIO 25   | PWMA     |
| GPIO 26   | PWMB     |
| GPIO 23   | STBY     |

### Ultrasonic Sensor

| ESP32 Pin | Function |
| --------- | -------- |
| GPIO 32   | Trigger  |
| GPIO 33   | Echo     |

### Light Sensors

| ESP32 Pin | Function  |
| --------- | --------- |
| GPIO 34   | Left LDR  |
| GPIO 35   | Right LDR |

---

## 📱 Blynk Controls

| Virtual Pin | Function                |
| ----------- | ----------------------- |
| V0          | Forward                 |
| V1          | Right                   |
| V2          | Backward                |
| V3          | Left                    |
| V4          | Obstacle Avoidance Mode |
| V5          | Light Following Mode    |

---

## ⚙️ Software Requirements

* Arduino IDE or PlatformIO
* ESP32 Board Package
* Blynk Library

Required libraries:
#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
---

## 🔑 Credentials Setup

Create a file named:

```cpp
private_credentials.h
```

Add the following content:

```cpp
#ifndef PRIVATE_CREDENTIALS_H
#define PRIVATE_CREDENTIALS_H

#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

#endif
```



### 2. Open the Project

Open the project using:

* Arduino IDE
* PlatformIO (recommended)


### 4. Upload to ESP32

Select your ESP32 board and upload the firmware.

### 5. Configure Blynk Dashboard

Create the following controls:

| Widget | Virtual Pin |
| ------ | ----------- |
| Button | V0          |
| Button | V1          |
| Button | V2          |
| Button | V3          |
| Switch | V4          |
| Switch | V5          |

## 🧠 How It Works

### Manual Mode

The ESP32 receives commands from the Blynk app and directly controls the motors.

### Obstacle Avoidance Mode

The ultrasonic sensor continuously measures the distance ahead. If an obstacle is detected within 15 cm, the car automatically performs an avoidance maneuver.

### Light Following Mode

The two LDR sensors compare light intensity. The car adjusts its steering to move toward the strongest light source.

---

## 🔮 Future Improvements

* Bluetooth control mode
* Camera streaming
* Line-following mode
* PID steering control
* Battery voltage monitoring
* Automatic route mapping

---

## 👨‍💻 Authors

Developed by Muhammad Noorullah Qureshi and Walton Kokfung Yaputra as an ESP32-based robotics project.

---