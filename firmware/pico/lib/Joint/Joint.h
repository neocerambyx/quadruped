#pragma once
#include "Servo.h"

class Joint {
    private:
        Servo actuator;

        // physical parameters of this joint
        float offset_angle;
        float min_angle;
        float max_angle;
        bool  inverted;

    public:
        Joint(uint pin);

        void init();

        // set limits and offsets for this joint
        void configure(float min, float max, float offset, bool invert = false);

        // takes the IK angle and applies physical corrections
        void setAngle(float desired_angle);

        float getOffset() const { return offset_angle; }
};