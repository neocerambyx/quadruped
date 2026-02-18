#include <Leg.h>

Leg::Leg(Adafruit_PWMServoDriver* driver, LegConfig config)
    : _cfg(config), _coxa(driver, config.coxa), _femur(driver, config.femur), _tibia(driver, config.tibia) {
}

#include <Arduino.h>
#include <math.h>

// Helper to convert radians to degrees
float toDeg(float rad) {
    return rad * 180.0 / PI;
}

void Leg::moveTo(float x, float y, float z) {
    float r2 = x * x + z * z;

    float L_coxa = _cfg.coxaLength;
    float L1 = _cfg.femurLength;
    float L2 = _cfg.tibiaLength;

    float x_loc = sqrt(r2) - L_coxa;

    float coxa_rad = atan2(z, x);

    float phi = acos((x_loc * x_loc + y * y - L1 * L1 - L2 * L2) / (2 * L1 * L2));

    float hip_rad = atan2(y, x_loc) - atan2(L2 * sin(phi), L1 + L2 * cos(phi));

    float knee_deg = 180.0 - (phi * 180.0 / M_PI);

    float servo_coxa = 90.0 + (coxa_rad * 180.0 / M_PI);

    float servo_hip = 90.0 + (hip_rad * 180.0 / M_PI);

    float servo_knee = 90.0 + knee_deg - servo_hip;

    servo_coxa = constrain(servo_coxa, 0.0f, 180.0f);
    servo_hip = constrain(servo_hip, 0.0f, 180.0f);
    servo_knee = constrain(servo_knee, 0.0f, 180.0f);

    _coxa.setAngle(servo_coxa);
    _femur.setAngle(servo_hip);
    _tibia.setAngle(servo_knee);

    Serial.print("Target: (");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.print(z);
    Serial.print(") | Coxa: ");
    Serial.print(servo_coxa);
    Serial.print(" | Hip: ");
    Serial.print(servo_hip);
    Serial.print(" | Knee: ");
    Serial.println(servo_knee);
}

void Leg::home() {
    moveTo(_cfg.femurLength, _cfg.tibiaLength, 0);
}

void Leg::fold() {
    moveTo(0, _cfg.tibiaLength, _cfg.femurLength + _cfg.coxaLength);
}
