#ifndef POSTURE_NORMAL_H
#define POSTURE_NORMAL_H
#include <stdint.h>
class PostureNormal {
public:
    PostureNormal(float angleTolerance = 5.0f, float maxStaticGyro = 15.0f);
    void init();
    void reset();
    bool check(float pitchError, float rollError, float gyroMagnitude);
    bool isNormal() const;
private:
    float _angleTolerance;   // Ngưỡng góc lệch chuẩn (mặc định 5.0 độ)
    float _maxStaticGyro;    // Ngưỡng vận tốc góc phân biệt trạng thái tĩnh
    bool  _isNormalState;    // Cờ lưu trạng thái tư thế chuẩn hiện tại
};

#endif