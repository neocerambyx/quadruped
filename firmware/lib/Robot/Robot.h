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

private:
    Adafruit_PWMServoDriver _pwm;
    bool _isWalking;
};