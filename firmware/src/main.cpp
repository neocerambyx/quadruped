#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Leg.h>
#include <Robot.h>
#include <Wire.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
constexpr uint32_t OSC_FREQ = 25500000;
constexpr uint8_t PWM_FREQ = 50;

constexpr uint16_t SERVO_MIN_US = 560;
constexpr uint16_t SERVO_MID_US = 1610;  // for reference
constexpr uint16_t SERVO_MAX_US = 2600;

constexpr float COXA_LENGTH = 0.0f;
constexpr float FEMUR_LENGTH = 10.0f;
constexpr float TIBIA_LENGTH = 10.0f;

const LegConfig FRONT_LEFT_CONF = {
    .coxa = {.id = 0, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 0.0f, .maxAngle = 180.0f, .offset = 0.0f, .inverted = false},
    .femur = {.id = 1, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 12.0f, .maxAngle = 180.0f, .offset = 0.0f, .inverted = true},
    .tibia = {.id = 2, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 0.0f, .maxAngle = 110.0f, .offset = 0.0f, .inverted = false},
    .coxaLength = COXA_LENGTH,
    .femurLength = FEMUR_LENGTH,
    .tibiaLength = TIBIA_LENGTH,
};

const LegConfig REAR_LEFT_CONF = {

    .coxa = {.id = 4, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 0.0f, .maxAngle = 180.0f, .offset = 0.0f, .inverted = false},
    .femur = {.id = 6, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 12.0f, .maxAngle = 180.0f, .offset = -7.2f, .inverted = true},
    .tibia = {.id = 5, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 0.0f, .maxAngle = 110.0f, .offset = 3.6f, .inverted = false},
    .coxaLength = COXA_LENGTH,
    .femurLength = FEMUR_LENGTH,
    .tibiaLength = TIBIA_LENGTH,
};

const LegConfig FRONT_RIGHT_CONF = {

    .coxa = {.id = 12, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 0.0f, .maxAngle = 180.0f, .offset = -7.2f, .inverted = false},
    .femur =
        {.id = 13, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 12.0f, .maxAngle = 180.0f, .offset = -7.2f, .inverted = false},
    .tibia = {.id = 14, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 0.0f, .maxAngle = 110.0f, .offset = 0.0f, .inverted = true},
    .coxaLength = COXA_LENGTH,
    .femurLength = FEMUR_LENGTH,
    .tibiaLength = TIBIA_LENGTH,
};

const LegConfig REAR_RIGHT_CONF = {

    .coxa = {.id = 8, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 0.0f, .maxAngle = 180.0f, .offset = 0.0f, .inverted = false},
    .femur = {.id = 9, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 12.0f, .maxAngle = 180.0f, .offset = 7.2f, .inverted = false},
    .tibia = {.id = 10, .minPulse = SERVO_MIN_US, .maxPulse = SERVO_MAX_US, .minAngle = 0.0f, .maxAngle = 110.0f, .offset = -3.6f, .inverted = true},
    .coxaLength = COXA_LENGTH,
    .femurLength = FEMUR_LENGTH,
    .tibiaLength = TIBIA_LENGTH,
};

Leg FL(&pwm, FRONT_LEFT_CONF);

Leg RL(&pwm, REAR_LEFT_CONF);

Leg FR(&pwm, FRONT_RIGHT_CONF);

Leg RR(&pwm, REAR_RIGHT_CONF);

Robot robot(&FL, &RL, &FR, &RR);


void setup() {
    Serial.begin(115200);
    delay(1000);

    pwm.begin();
    pwm.setOscillatorFrequency(OSC_FREQ);
    pwm.setPWMFreq(PWM_FREQ);
    delay(10);  // let oscillator settle

    Serial.println("Initializing...");

    robot.init();
    Serial.println("Ready.");
}


void loop() {

    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input == "home") {
            robot.home();

            Serial.println("Homed.");

        } else if (input == "stand") {
            robot.stand();
            Serial.println("Standing.");

        } else if (input == "crouch") {
            robot.crouch();

            Serial.println("crouching.");

        } else if (input == "walk") {
            Serial.println("Walking 10 steps...");
            robot.trot(10);

        } else {
            // // still support manual coordinate entry
            // float x, y, z;
            // int first = input.indexOf(',');
            // int second = input.indexOf(',', first + 1);
            // if (first == -1 || second == -1) {
            //     Serial.println("Commands: walk, crouch, stand, home, or x,y,z");
            //     return;
            // }
            // x = input.substring(0, first).toFloat();
            // y = input.substring(first + 1, second).toFloat();
            // z = input.substring(second + 1).toFloat();
            // Serial.print("Moving to: ");
            // Serial.print(x);
            // Serial.print(", ");
            // Serial.print(y);
            // Serial.print(", ");
            // Serial.println(z);
            // RL.moveToSmooth(x, y, z);
    //     }
    }
}