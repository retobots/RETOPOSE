#ifndef POSTURE_STRETCH_H
#define POSTURE_STRETCH_H
#include <stdint.h>
class PostureStretch {
public:
    PostureStretch(float leanBackThresh = -10.0f, uint32_t maxRelaxDurationMs = 60000);

    void init();
    void reset();
    bool check(float pitchRelative, uint32_t currentTimeMs);
    bool isRelaxing() const;
private:
    float _leanBackThresh;
    uint32_t _maxRelaxDurationMs;

    bool _isRelaxing;
    uint32_t _relaxStartTime;
};

#endif // POSTURE_STRETCH_H