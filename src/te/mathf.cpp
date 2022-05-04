#include "te/mathf.hpp"

namespace te {

Mat4::Mat4() {
    memset(data_, 0, sizeof(data_));
}

Mat4::Mat4(const std::array<Vector4, 4>& cols) {
    for (int i = 0; i < 4; i++) {
        Set(i, 0, cols[i].x);
        Set(i, 1, cols[i].y);
        Set(i, 2, cols[i].z);
        Set(i, 3, cols[i].w);
    }
}

Mat4::Mat4(const std::initializer_list<float>& list) {
    int i = 0;
    for (auto value : list) {
        data_[i ++] = value;
    }
}

Mat4 CreateSRT(const Vector2& pos, const Vector2& size, float rotation) {
    float radians = Radians(rotation),
          cos = std::cos(radians),
          sin = std::sin(radians);
    return Mat4({
             size.w * cos, -size.w * sin, 0, 0,
             size.h * sin, size.h * cos, 0, 0,
            0, 0, 1, 0,
            pos.x, pos.y, 0, 1
            });
}

Mat4 CreateOrtho(float left, float right, float bottom, float top, float near, float far) {
    return Mat4{
              2 / (right - left), 0, 0, 0,
              0, 2 / (top - bottom), 0, 0,
              0, 0, 2 / (near - far), 0,
              (right + left) / (left - right), (top + bottom) / (bottom - top), (near + far) / (far - near), 1
            };
}

Mat4 CreateEyeMat(float value) {
    return Mat4{
        value, 0, 0, 0,
            0, value, 0, 0,
            0, 0, value, 0,
            0, 0, 0, value
    };
}

}
