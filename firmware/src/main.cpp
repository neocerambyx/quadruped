#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Leg.h>
#include <Wire.h>
#include <math.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo Constants
constexpr uint16_t SERVOMIN = 140;  // Min. Pulse Width (µs)
constexpr uint16_t SERVOMAX = 520;  // Max. Pulse Width (µs)
constexpr uint8_t FREQ = 50;        // PWM Period (Hz)

// Mechanical Constants
constexpr float COXA_LENGTH = 2.9f;   // Hip adduction/abduction
constexpr float FEMUR_LENGTH = 6.0f;  // Hip flexion
constexpr float TIBIA_LENGTH = 8.0f;  // Knee flexion

const LegConfig FRONT_LEFT_CONF = {

    .coxa = {.id = 0, .minPulse = SERVOMIN, .maxPulse = SERVOMAX, .minAngle = 0.0f, .maxAngle = 180.0f, .offset = 0.0f, .inverted = false},

    .femur = {.id = 1, .minPulse = SERVOMIN, .maxPulse = SERVOMAX, .minAngle = 12.0f, .maxAngle = 180.0f, .offset = 5.0f, .inverted = true},  // TODO

    .tibia = {.id = 2, .minPulse = SERVOMIN, .maxPulse = SERVOMAX, .minAngle = 0.0f, .maxAngle = 110.0f, .inverted = false},

    .coxaLength = COXA_LENGTH,
    .femurLength = FEMUR_LENGTH,
    .tibiaLength = TIBIA_LENGTH};

Leg testLeg(&pwm, FRONT_LEFT_CONF);

// Leg State
float footX = 0;
float footY = 0;
float footZ = 0;

void setup() {
    Serial.begin(115200);

    delay(1000);
    Serial.println("Initialized. \n");

    pwm.begin();
    pwm.setOscillatorFrequency(27000000);  // 25MHz internal clock but ~27MHz actual recommended by manufacturer
    // Need oscilliscope to get chip's precise unique freq.; don't have one yet
    pwm.setPWMFreq(FREQ);

    Serial.println("Homing...");
    testLeg.home();
    delay(2000);
    testLeg.fold();
    // 0,8,9 for home rotated 90 degrees
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        int firstSep = input.indexOf(',');
        int secondSep = input.indexOf(',', firstSep + 1);

        if (firstSep > 0 && secondSep > firstSep) {
            float x = input.substring(0, firstSep).toFloat();
            float y = input.substring(firstSep + 1, secondSep).toFloat();
            float z = input.substring(secondSep + 1).toFloat();

            Serial.print("Moving to: ");
            Serial.print(x);
            Serial.print(", ");
            Serial.print(y);
            Serial.print(", ");
            Serial.println(z);

            testLeg.moveTo(x, y, z);
        }
    }
}
