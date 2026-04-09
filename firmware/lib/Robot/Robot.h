#pragma once
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>

#include "Leg.h"

class Robot {
public:
    Robot(Leg* fl, Leg* rl, Leg* fr, Leg* rr);
    void init();
    void home();
    void stand();
    void crouch();

    void trot(int steps);

    Leg* frontLeft;
    Leg* frontRight;
    Leg* backLeft;
    Leg* backRight;

    // pitchDeg: + looks up, - looks down
    // rollDeg: + tilts right, - tilts left
    // zOffset: + lowers body, - raises body
    void setStance(float pitchDeg, float rollDeg, float zOffset = 0.0f);

private:
    Adafruit_PWMServoDriver _pwm;
    bool _isWalking;

    float _restX = 0.5f;   // Center X
    float _restY = 12.0f;  // Standing Height
    float _restZ = 0.0f;   // Lateral Spread
    float _liftY = 8.0f;   // Lifted height
    float _stride = 4.0f;
    unsigned long _stepDelayMs = 150;

float _bodyLength = 15.0f;
float _bodyWidth = 12.0f;
};