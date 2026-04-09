#include "Robot.h"

Robot::Robot(Leg* fl, Leg* rl, Leg* fr, Leg* rr) : frontLeft(fl), backLeft(rl), frontRight(fr), backRight(rr), _isWalking(false) {
}

void Robot::init() {
    crouch();
    delay(2000);
    stand();
    delay(2000);
}

void Robot::home() {
    _isWalking = false;
    frontLeft->home();
    backLeft->home();
    frontRight->home();
    backRight->home();
}

void Robot::stand() {
    _isWalking = false;
    frontLeft->stand();
    backLeft->stand();
    frontRight->stand();
    backRight->stand();
}

void Robot::crouch() {
    frontLeft->crouch();
    backLeft->crouch();
    frontRight->crouch();
    backRight->crouch();
}

// Trot gait generator
// use sine and cosine wave to generate semi-circular step trajectories
// diagonal leg pairs move 180 degrees out of phase

void Robot::trot(int steps) {
    float y_stand = 14.0f;  // standing height
    float y_lift = 11.0f;    // height at peak of step
    float x_stride = 6.0f;  // forward/backward stride distance
    float x_offset = 0.5f;  // centers stride exactly standing position
    float z_pos = 0.0f;

    for (int i = 0; i < steps; i++) {
        // start t at PI/2 so the cycle begins exactly at x_offset
        // so transition from standing to walking is smooth
        for (float t = PI / 2; t <= 2.5f * PI; t += 0.15f) {
            // FL & RR
            float leg1_x = x_offset + x_stride * cos(t);
            float leg1_y = y_stand;
            if (sin(t) > 0) {
                // Lift leg upwards while swinging forward
                leg1_y -= (y_stand - y_lift) * sin(t);
            }

            // FR & RL
            float leg2_x = x_offset + x_stride * cos(t + PI);
            float leg2_y = y_stand;
            if (sin(t + PI) > 0) {
                // lift leg upwards while swinging forward
                leg2_y -= (y_stand - y_lift) * sin(t + PI);
            }

            frontLeft->moveTo(leg1_x, leg1_y, z_pos);
            backRight->moveTo(leg1_x, leg1_y, z_pos);

            frontRight->moveTo(leg2_x, leg2_y, z_pos);
            backLeft->moveTo(leg2_x, leg2_y, z_pos);

            delay(5);
        }
    }

    // return to neutral stance at end of trot
    frontLeft->stand();
    backLeft->stand();
    frontRight->stand();
    backRight->stand();
}

void Robot::setStance(float pitchDeg, float rollDeg, float yOffset) {
    // stop walking if we are posing
    _isWalking = false;

    // convert degrees to radians
    float pitchRad = pitchDeg * PI / 180.0f;
    float rollRad = rollDeg * PI / 180.0f;

    // calculate how much Y to change for the front/back and left/right

    float frontYOffset = (_bodyLength / 2.0f) * sin(pitchRad);
    float backYOffset  = -(_bodyLength / 2.0f) * sin(pitchRad);

    float leftYOffset  = -(_bodyWidth / 2.0f) * sin(rollRad);
    float rightYOffset = (_bodyWidth / 2.0f) * sin(rollRad);

    // calculate final target Y for each leg by combining offsets
    float fl_y = _restY + yOffset + frontYOffset + leftYOffset;
    float fr_y = _restY + yOffset + frontYOffset + rightYOffset;
    float bl_y = _restY + yOffset + backYOffset + leftYOffset;
    float br_y = _restY + yOffset + backYOffset + rightYOffset;

    // command legs to move to new stance 
    frontLeft->moveTo(_restX, fl_y, _restZ);
    frontRight->moveTo(_restX, fr_y, _restZ);
    backLeft->moveTo(_restX, bl_y, _restZ);
    backRight->moveTo(_restX, br_y, _restZ);
}