# IoT Autonomous Robot
This project is a line-tracing robotic car using an **ESP32-WROOM-32** and **Blynk**.

## Hardware
* ESP32-WROOM-32
* L298N Motor Driver
* IR Line Sensors
* (Coming Soon) Ultrasonic/More sensors

## Setup
1. Create a file in the `src` folder named `private_credentials.h`.
2. Add your WiFi and Blynk credentials there (refer to `main.cpp`).
3. Use PlatformIO to upload the code to the ESP32.