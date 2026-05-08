#pragma once
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <cstdint>

class Servo {
private:
    uint _pin;
    uint _slice_num = pwm_gpio_to_slice_num(_pin);
    uint _channel;
    uint16_t _min_pulse = 500;
    uint16_t _max_pulse = 2500;

public:
    void init();
    void setPulse(uint16_t pulse_us);
    void setAngle(float angle);
};