#include <Joint.h>

Joint::Joint(Adafruit_PWMServoDriver* driver, JointConfig config) : _driver(driver), _cfg(config) {
}

void Joint::setAngle(float angle) {
    if (!isEnabled) return; 

    angle = constrain(angle, _cfg.minAngle, _cfg.maxAngle);
    
    _currentAngle = angle;
    _driver->setPWM(_cfg.id, 0, degreesToPulse(angle));

}

//Filtered_Angle=(α×New_Target)+((1−α)×Previous_Filtered_Angle)


void Joint::disable() {
    isEnabled = false;
    _driver->setPWM(_cfg.id, 0, 4096);
}

void Joint::enable() {
    isEnabled = true;
}

uint16_t Joint::degreesToPulse(float angle) {
    angle = constrain(angle, _cfg.minAngle, _cfg.maxAngle);
    if (_cfg.inverted) angle = 180.0f - angle;
    angle = constrain(angle + _cfg.offset, 0.0f, 180.0f);

    // float lerp
    uint16_t us = (uint16_t)(_cfg.minPulse + (_cfg.maxPulse - _cfg.minPulse) * (angle / 180.0f) + 0.5f);

    // PCA9685 wants 12-bit ticks instead of us
    // at 50 Hz period = 20000µs, 4096 ticks, tick = us * 4096 / 20000
    return (uint16_t)((uint32_t)us * 4096UL / 20000UL);
}