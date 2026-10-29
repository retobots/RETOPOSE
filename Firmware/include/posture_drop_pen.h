#ifndef POSTURE_DROP_PEN_H //ktra có  định nghĩa POSTURE_DROP_PEN_H chưa
#define POSTURE_DROP_PEN_H //chưa thì sẽ định nghĩa POSTURE_DROP_PEN_H
#include <cstdint>
class PostureDropPen {
public:
PostureDropPen(float angleThreshold = 45.0f, 
                   float dynamicGyroThreshold = 35.0f, 
                   uint32_t maxDurationMs = 3000);
 void init();
 void reset();
 bool check(float pitchError, float gyroMagnitude, uint32_t currentTimeMs);
 bool isDroppingActive() const;
 private:
    float _angleThreshold;
    float _dynamicGyroThreshold;
    uint32_t _maxDurationMs;

    bool _isActive;
    uint32_t _startTimestamp;
};
#endif