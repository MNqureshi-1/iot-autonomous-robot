#define BLYNK_TEMPLATE_ID "TMPL4SDzs183M"
#define BLYNK_TEMPLATE_NAME "Walli and noor RC car dinge"

#include "private_credentials.h"




#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>



// ---------------- MOTOR PINS ----------------
const int AIN1 = 18;
const int AIN2 = 19;
const int BIN1 = 21;
const int BIN2 = 22;
const int PWMA = 25;
const int PWMB = 26;
const int STBY = 23;

// ---------------- ULTRASONIC SENSOR ----------------
const int trigPin = 32;
const int echoPin = 33;

// ---------------- PWM ----------------
const int freq = 1000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int resolution = 8;

// ---------------- SPEEDS ----------------
int speedValue = 200;
int turnSpeed = 120;
int avoidSpeed = 180;

// ---------------- OBSTACLE SETTINGS ----------------
const int obstacleDistanceCm = 15;
const int maxValidDistanceCm = 300;

// ---------------- STATES ----------------
bool forwardPressed = false;
bool backwardPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool obstacleMode = false;

// ---------------- DEBUG TIMER ----------------
unsigned long lastDebugPrint = 0;

// ---------------- MOTOR HELPERS ----------------
void setSpeed(int leftMotorSpeed, int rightMotorSpeed) {
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

  ledcWrite(pwmChannelA, leftMotorSpeed);
  ledcWrite(pwmChannelB, rightMotorSpeed);
}

void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  setSpeed(0, 0);
}

void driveForward(int leftMotorSpeed, int rightMotorSpeed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  setSpeed(leftMotorSpeed, rightMotorSpeed);
}

void driveBackward(int leftMotorSpeed, int rightMotorSpeed) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  setSpeed(leftMotorSpeed, rightMotorSpeed);
}

void spinRight() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  setSpeed(speedValue, speedValue);
}

void spinLeft() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  setSpeed(speedValue, speedValue);
}

// ---------------- ULTRASONIC ----------------
long readDistanceCm() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  long distanceCm = duration * 0.0343 / 2.0;
  return distanceCm;
}

bool obstacleDetected() {
  long distance = readDistanceCm();

  if (millis() - lastDebugPrint > 250) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    lastDebugPrint = millis();
  }

  if (distance == -1) return false;
  if (distance > maxValidDistanceCm) return false;

  return distance <= obstacleDistanceCm;
}

// ---------------- MANUAL CONTROL ----------------
void updateManualMovement() {
  if (forwardPressed && backwardPressed) {
    stopMotors();
    return;
  }

  if (forwardPressed) {
    if (rightPressed && !leftPressed) {
      driveForward(speedValue, turnSpeed);
    } else if (leftPressed && !rightPressed) {
      driveForward(turnSpeed, speedValue);
    } else {
      driveForward(speedValue, speedValue);
    }
    return;
  }

  if (backwardPressed) {
    if (rightPressed && !leftPressed) {
      driveBackward(speedValue, turnSpeed);
    } else if (leftPressed && !rightPressed) {
      driveBackward(turnSpeed, speedValue);
    } else {
      driveBackward(speedValue, speedValue);
    }
    return;
  }

  if (rightPressed && !leftPressed) {
    spinRight();
    return;
  }

  if (leftPressed && !rightPressed) {
    spinLeft();
    return;
  }

  stopMotors();
}

// ---------------- OBSTACLE AVOIDANCE ----------------
void avoidObstacle() {
  stopMotors();
  delay(150);

  driveBackward(avoidSpeed, avoidSpeed);
  delay(300);

  stopMotors();
  delay(100);

  spinRight();
  delay(350);

  stopMotors();
  delay(100);
}

void runObstacleMode() {
  // Car moves forward by itself
  if (obstacleDetected()) {
    Serial.println("Obstacle detected!");
    avoidObstacle();
  } else {
    driveForward(speedValue, speedValue);
  }
}

// ---------------- BLYNK ----------------
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0, V1, V2, V3, V4);
}

BLYNK_WRITE(V0) {   // Forward
  forwardPressed = (param.asInt() == 1);
  if (!obstacleMode) updateManualMovement();
}

BLYNK_WRITE(V1) {   // Right
  rightPressed = (param.asInt() == 1);
  if (!obstacleMode) updateManualMovement();
}

BLYNK_WRITE(V2) {   // Backward
  backwardPressed = (param.asInt() == 1);
  if (!obstacleMode) updateManualMovement();
}

BLYNK_WRITE(V3) {   // Left
  leftPressed = (param.asInt() == 1);
  if (!obstacleMode) updateManualMovement();
}

BLYNK_WRITE(V4) {   // Obstacle avoidance ON/OFF
  obstacleMode = (param.asInt() == 1);

  Serial.print("ObstacleMode = ");
  Serial.println(obstacleMode);

  stopMotors();

  if (!obstacleMode) {
    updateManualMovement();
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(STBY, HIGH);

  ledcSetup(pwmChannelA, freq, resolution);
  ledcAttachPin(PWMA, pwmChannelA);

  ledcSetup(pwmChannelB, freq, resolution);
  ledcAttachPin(PWMB, pwmChannelB);

  stopMotors();

  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Setup complete");
}

// ---------------- LOOP ----------------
void loop() {
  Blynk.run();

  int distance2=readDistanceCm();

  Serial.println(distance2);

  if (obstacleMode) {
    runObstacleMode();
  }


} 