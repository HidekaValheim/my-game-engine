// include/engine/math/Math.h
#pragma once
#include <cmath>
#include <cstring>

namespace engine::math {

    struct Vec3 {
        float x, y, z;
        Vec3(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) {}
    };

    struct Mat4 {
        float m[16];
        Mat4() { std::memset(m, 0, sizeof(m)); m[0] = m[5] = m[10] = m[15] = 1.0f; }
    };

    // Core functions — raw float* only
    void mul(float* out, const float* a, const float* b);
    Mat4 perspective(float fov, float aspect, float nearZ, float farZ);
    Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    Mat4 translate(const Mat4& m, const Vec3& v);
    Mat4 rotate(const Mat4& m, float angle, const Vec3& axis);
    Mat4 scale(const Mat4& m, const Vec3& v);

    // Helper: convert Mat4 to float*
    inline const float* value_ptr(const Mat4& mat) { return mat.m; }

} // namespace engine::math