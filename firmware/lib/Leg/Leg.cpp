#include <Arduino.h>
#include <Leg.h>
#include <math.h>

Leg::Leg(Adafruit_PWMServoDriver* driver, LegConfig config)
    : _cfg(config), _coxa(driver, config.coxa), _femur(driver, config.femur), _tibia(driver, config.tibia) {
}

static inline float toDeg(float rad) {
    return rad * 180.0f / PI;
}

void Leg::solveIK(float x, float y, float z, float& outCoxa, float& outFemur, float& outTibia) {
    float r_yz  = sqrt(y * y + z * z);
    float L1    = _cfg.femurLength;
    float L2    = _cfg.tibiaLength;
    float Lc    = _cfg.coxaLength;

    float y_loc   = r_yz - Lc;
    float coxa_rad = atan2(z, y);
    float cos_phi  = constrain((x*x + y_loc*y_loc - L1*L1 - L2*L2) / (2.0f*L1*L2), -1.0f, 1.0f);
    float phi      = acos(cos_phi);
    float hip_rad  = atan2(y_loc, x) - atan2(L2*sin(phi), L1 + L2*cos(phi));
    float knee_deg = 180.0f - toDeg(phi);

    outCoxa  = 90.0f + toDeg(coxa_rad);
    outFemur = 90.0f + toDeg(hip_rad);
    outTibia = 90.0f + knee_deg - outFemur;
}

void Leg::moveTo(float x, float y, float z) {
    float c, f, t;
    solveIK(x, y, z, c, f, t);

    _coxa.setAngle(c);
    _femur.setAngle(f);
    _tibia.setAngle(t);

    // uncomment for single-step debugging.
    /*
    Serial.print("Target: (");
    Serial.print(x); Serial.print(", ");
    Serial.print(y); Serial.print(", ");
    Serial.print(z); Serial.print(") | Coxa: ");
    Serial.print(c); Serial.print(" | Hip: ");
    Serial.print(f); Serial.print(" | Knee: ");
    Serial.println(t);
    */
}
void Leg::home() {
    moveTo(_cfg.femurLength, _cfg.tibiaLength, 0.0f);
}

void Leg::stand() {
    moveTo(0.5,10,0);
}

void Leg::crouch() {
    moveTo(-1,8,0);
}

void Leg::fold() {
    moveTo(-_cfg.tibiaLength, 0.0f, _cfg.femurLength);
}

void Leg::moveToSmooth(float x, float y, float z, float maxDegPerSec, uint16_t stepMs) {
    float endCoxa, endFemur, endTibia;
    solveIK(x, y, z, endCoxa, endFemur, endTibia);

    float curCoxa  = _coxa.getAngle();
    float curFemur = _femur.getAngle();
    float curTibia = _tibia.getAngle();

    float maxDegPerStep = maxDegPerSec * ((float)stepMs / 1000.0f);
    while (true) {
        bool done = true;

        auto step = [&](float cur, float end) -> float {
            float diff = end - cur;
            if (abs(diff) <= maxDegPerStep) return end;
            done = false;
            return cur + (diff > 0 ? maxDegPerStep : -maxDegPerStep);
        };

        curCoxa  = step(curCoxa,  endCoxa);
        curFemur = step(curFemur, endFemur);
        curTibia = step(curTibia, endTibia);

        _coxa.setAngle(curCoxa);
        _femur.setAngle(curFemur);
        _tibia.setAngle(curTibia);

        if (done) break;
        delay(stepMs);
    }
}