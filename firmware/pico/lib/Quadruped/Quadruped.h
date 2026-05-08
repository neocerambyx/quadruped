#pragma once
#include "Leg.h"

class Quadruped {
private:
    Leg front_left;
    Leg front_right;
    Leg back_left;
    Leg back_right;

    // physical parameters (mm)
    const float body_length = 15.0f;
    const float body_width = 12.0f;

    bool _isWalking;
public:
    Quadruped();

    void home();
    void init();
    void stand();
    void crouch();

    void trot(int steps);

    // state machine to sequence footsteps over time
    void updateGait(uint32_t current_time_ms);
};