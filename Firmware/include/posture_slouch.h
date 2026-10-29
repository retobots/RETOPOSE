#ifndef POSTURE_SLOUCH_H
#define POSTURE_SLOUCH_H
#include <stdint.h>
class PostureSlouch {
public:
    PostureSlouch(float slouchAngleThresh = 15.0f, uint32_t debounceDurationMs = 5000);

    void init();
    void reset();
    bool check(float pitchError, uint32_t currentTimeMs);
    bool isSlouching() const;
private:
    float _slouchAngleThresh;
    uint32_t _debounceDurationMs;
    bool _isSlouching;
    uint32_t _slouchStartTime;
};

#endif