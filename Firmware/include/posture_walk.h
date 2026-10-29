#ifndef POSTURE_WALK_H
#define POSTURE_WALK_H
#include <stdint.h>
class PostureWalk {
public:
    PostureWalk(float accelLowThresh = 0.75f, 
                float accelHighThresh = 1.30f, 
                float gyroWalkThresh = 30.0f, 
                uint32_t standbyTimeoutMs = 2500);
    void init();
    void reset();
    bool check(float totalAccel, float gyroMagnitude, uint32_t currentTimeMs);
    bool isStandby() const;
private:
    float _accelLowThresh;
    float _accelHighThresh;
    float _gyroWalkThresh;
    uint32_t _standbyTimeoutMs;
    bool _isStandby;
    uint32_t _lastMovementTimestamp;
};

#endif