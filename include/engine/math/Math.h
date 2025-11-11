#pragma once
#include <cmath>

namespace engine::math {

    struct Vec3 {
        float x, y, z;
        Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    };

    struct Mat4 {
        float m[16] = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    };

    Mat4 perspective(float fov, float aspect, float near, float far);
    Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    Mat4 translate(const Mat4& m, const Vec3& v);
    Mat4 rotate(const Mat4& m, float angle, const Vec3& axis);
    Mat4 scale(const Mat4& m, const Vec3& v);
    void mul(float* out, const float* a, const float* b);

}  // namespace engine::math