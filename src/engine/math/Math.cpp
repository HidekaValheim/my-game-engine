// src/engine/math/Math.cpp
#include "engine/math/Math.h"

namespace engine::math {

    void mul(float* out, const float* a, const float* b) {
        float temp[16];
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                temp[i * 4 + j] = 0;
                for (int k = 0; k < 4; ++k)
                    temp[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
            }
        std::memcpy(out, temp, sizeof(temp));
    }

    Mat4 perspective(float fov, float aspect, float nearZ, float farZ) {
        Mat4 result;
        float f = 1.0f / tanf(fov * 0.5f);
        std::memset(result.m, 0, sizeof(result.m));
        result.m[0] = f / aspect;
        result.m[5] = f;
        result.m[10] = (farZ + nearZ) / (nearZ - farZ);
        result.m[11] = -1.0f;
        result.m[14] = (2 * farZ * nearZ) / (nearZ - farZ);
        return result;
    }

    Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
        Vec3 f(center.x - eye.x, center.y - eye.y, center.z - eye.z);
        float len = std::sqrt(f.x * f.x + f.y * f.y + f.z * f.z);
        if (len > 0.0f) f = Vec3(f.x / len, f.y / len, f.z / len);

        Vec3 s(f.y * up.z - f.z * up.y, f.z * up.x - f.x * up.z, f.x * up.y - f.y * up.x);
        len = std::sqrt(s.x * s.x + s.y * s.y + s.z * s.z);
        if (len > 0.0f) s = Vec3(s.x / len, s.y / len, s.z / len);

        Vec3 u(s.y * f.z - s.z * f.y, s.z * f.x - s.x * f.z, s.x * f.y - s.y * f.x);

        Mat4 result;
        result.m[0] = s.x; result.m[1] = u.x; result.m[2] = -f.x;
        result.m[4] = s.y; result.m[5] = u.y; result.m[6] = -f.y;
        result.m[8] = s.z; result.m[9] = u.z; result.m[10] = -f.z;
        result.m[12] = -(s.x * eye.x + s.y * eye.y + s.z * eye.z);
        result.m[13] = -(u.x * eye.x + u.y * eye.y + u.z * eye.z);
        result.m[14] = f.x * eye.x + f.y * eye.y + f.z * eye.z;
        result.m[15] = 1.0f;
        return result;
    }

    Mat4 translate(const Mat4& m, const Vec3& v) {
        Mat4 result = m;
        result.m[12] = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12];
        result.m[13] = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13];
        result.m[14] = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
        result.m[15] = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15];
        return result;
    }

    Mat4 rotate(const Mat4& m, float angle, const Vec3& axis) {
        float c = cosf(angle), s = sinf(angle);
        Vec3 a = axis;
        float len = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
        if (len > 0.0f) { a.x /= len; a.y /= len; a.z /= len; }

        Mat4 rot;
        rot.m[0] = c + a.x * a.x * (1 - c);    rot.m[4] = a.x * a.y * (1 - c) - a.z * s; rot.m[8] = a.x * a.z * (1 - c) + a.y * s;
        rot.m[1] = a.y * a.x * (1 - c) + a.z * s; rot.m[5] = c + a.y * a.y * (1 - c);     rot.m[9] = a.y * a.z * (1 - c) - a.x * s;
        rot.m[2] = a.z * a.x * (1 - c) - a.y * s; rot.m[6] = a.z * a.y * (1 - c) + a.x * s; rot.m[10] = c + a.z * a.z * (1 - c);
        rot.m[15] = 1.0f;

        Mat4 result;
        mul(result.m, m.m, rot.m);
        return result;
    }

    Mat4 scale(const Mat4& m, const Vec3& v) {
        Mat4 result = m;
        for (int i = 0; i < 4; ++i) result.m[i] *= v.x;
        for (int i = 4; i < 8; ++i) result.m[i] *= v.y;
        for (int i = 8; i < 12; ++i) result.m[i] *= v.z;
        return result;
    }

} // namespace