#pragma once

#include "pch.hpp"
#include <sys/wait.h>

namespace te {

/******************
 * Vector22
******************/
struct Vector2 {
    union {
        float data[2];
        struct { float x, y; };
        struct { float w, h; };
    };

    Vector2(): x(0), y(0) {}

    Vector2(float x, float y): x(x), y(y) {}

    explicit Vector2(float value): x(value), y(value) {}

    size_t Size() const { return 2; }

    void Set(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vector2& operator=(const Vector2& o) {
        x = o.x;
        y = o.y;
        return *this;
    }

    Vector2& operator+=(const Vector2& o) {
        x += o.x;
        y += o.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& o) {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    Vector2& operator*=(const Vector2& o) {
        x *= o.x;
        y *= o.y;
        return *this;
    }

    bool operator==(const Vector2& o) const {
        return x == o.x && y == o.y;
    }

    bool operator!=(const Vector2& o) const {
        return !(*this == o);
    }

    bool Equal(const Vector2& o) const {
        auto epsilon = std::numeric_limits<float>::epsilon();
        return std::abs(o.x - x) <= epsilon &&
               std::abs(o.y - y) <= epsilon;
    }

    Vector2& operator/=(const Vector2& o) {
        x /= o.x;
        y /= o.y;
        return *this;
    }

    float Len2() const {
        return x * x + y * y;
    }

    float Len() const {
        return std::sqrt(Len2());
    }
};

inline Vector2 operator+(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}

inline Vector2 operator-(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}

inline Vector2 operator*(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x * v2.x, v1.y * v2.y);
}

inline auto operator*(const Vector2& v, float value) {
    return Vector2(v.x * value, v.y * value);
}

inline auto operator*(float value, const Vector2& v) {
    return v * value;
}

inline Vector2 operator/(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x / v2.x, v1.y / v2.y);
}

inline auto operator/(const Vector2& v, float value) {
    return Vector2(v.x / value, v.y / value);
}

inline Vector2 Normalize(const Vector2& v) {
    float len = v.Len();
    return v / (float)len;
}

inline float Dot(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

inline float Cross(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

using Vec2 = Vector2;
using Size = Vector2;

struct Vector4 final {
    union {
        struct { float r, g, b, a; };
        struct { float x, y, z, w; };
    };

    Vector4(float x, float y, float z, float w = 1): x(x), y(y), z(z), w(w) {}
    Vector4(): r(0), g(0), b(0), a(0) {}
    void Set(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
};

using Vec4 = Vector4;
using Color = Vector4;

/*******************
 * Rect
*******************/
template <typename T>
struct RectBase final {
    union {
        struct { float x, y, w, h; };
    };

    RectBase(): x{}, y{}, w{}, h{} {}
    RectBase(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}

    void Set(float x, float y, float w, float h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
};

using Rect = RectBase<double>;
using Recti = RectBase<int>;

inline bool IsPointInRect(const Rect& r, const Vector2& p) {
    return p.x >= r.x && p.x <= r.x + r.w &&
           p.y >= r.y && p.y <= r.y + r.h;
}

inline bool IsPointInRect(const Recti& r, const Vector2& p) {
    return p.x >= r.x && p.x <= r.x + r.w &&
           p.y >= r.y && p.y <= r.y + r.h;
}

inline Vec2 AlignLeftPos(const Rect& target, const Rect& r) {
    return Vec2(target.x, r.y);
}

inline Vec2 AlignRightPos(const Rect& target, const Rect& r) {
    return Vec2(target.x + target.w - r.w, r.y);
}

inline Vec2 AlignfloatopPos(const Rect& target, const Rect& r) {
    return Vec2(r.x, target.y);
}

inline Vec2 AlignBottomPos(const Rect& target, const Rect& r) {
    return Vec2(r.x, target.y + target.h - r.h);
}

inline Vec2 AlignCenterXPos(const Rect& target, const Rect& r) {
    return Vec2(target.x + (target.w - r.w) / 2, r.y);
}

inline Vec2 AlignCenterYPos(const Rect& target, const Rect& r) {
    return Vec2(r.x, target.y + (target.h - r.h) / 2);
}


// Column first matrix
class Mat4 final {
public:
    Mat4();
    Mat4(const std::array<Vector4, 4>& cols);
    Mat4(const std::initializer_list<float>&);

    float Get(int x, int y) const { return data_[x * 4 + y]; }
    void Set(int x, int y, float value) { data_[x * 4 + y] = value; }
    float* Data() { return data_; }
    const float* Data() const { return data_; }

private:
    float data_[4 * 4];
};

Mat4 CreateSRT(const Vector2& pos, const Vector2& size_, float rotation);

constexpr float PI = 3.1415926536;

inline float Radians(float degrees) {
    return degrees * PI / 180.0;
}

inline float Degrees(float radians) {
    return radians * 180.0 / PI;
}

Mat4 CreateOrtho(float left, float right, float bottom, float top, float far, float near);
Mat4 CreateEyeMat(float value);

}
