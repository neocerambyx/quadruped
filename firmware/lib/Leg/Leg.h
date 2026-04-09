#pragma once
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Joint.h>

struct LegConfig {
    JointConfig coxa;   // Joint 1
    JointConfig femur;  // Joint 2
    JointConfig tibia;  // Joint 3

    float coxaLength;   // Link 1
    float femurLength;  // Link 2
    float tibiaLength;  // Link 3
};

class Leg {
public:
    Leg(Adafruit_PWMServoDriver* driver, LegConfig config);

    void moveTo(float x, float y, float z);

    void home();  // Return to safe position

    void stand();  // stand

    void crouch();

    void fold();  // Home abducted 90 deg.

    void moveToSmooth(float x, float y, float z, float maxDegPerSec = 120.0f, uint16_t stepMs = 10);

public:
    LegConfig _cfg;  // Store settings in class

    Joint _coxa;
    Joint _femur;
    Joint _tibia;
    bool isEnabled = true;
    void disable();
    void enable();
private:
    void solveIK(float x, float y, float z, float& outCoxa, float& outFemur, float& outTibia);
};
