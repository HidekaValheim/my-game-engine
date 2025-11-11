#include "engine/math/Math.h"
#include <cstring>

namespace engine::math {

    void mul(float* out, const float* a, const float* b) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                out[i * 4 + j] = 0;
                for (int k = 0; k < 4; ++k) {
                    out[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
                }
            }
        }
    }

    Mat4 perspective(float fov, float aspect, float near, float far) {
        Mat4 m;
        float f = 1.0f / tanf(fov * 0.5f);
        m.m[0] = f / aspect;
        m.m[5] = f;
        m.m[10] = (far + near) / (near - far);
        m.m[11] = -1.0f;
        m.m[14] = (2 * far * near) / (near - far);
        m.m[15] = 0;
        return m;
    }

    Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
        Vec3 f = { center.x - eye.x, center.y - eye.y, center.z - eye.z };
        float len = sqrtf(f.x * f.x + f.y * f.y + f.z * f.z);
        f.x /= len; f.y /= len; f.z /= len;

        Vec3 s = { f.y * up.z - f.z * up.y, f.z * up.x - f.x * up.z, f.x * up.y - f.y * up.x };
        len = sqrtf(s.x * s.x + s.y * s.y + s.z * s.z);
        s.x /= len; s.y /= len; s.z /= len;

        Vec3 u = { s.y * f.z - s.z * f.y, s.z * f.x - s.x * f.z, s.x * f.y - s.y * f.x };

        Mat4 m;
        m.m[0] = s.x; m.m[1] = u.x; m.m[2] = -f.x; m.m[3] = 0;
        m.m[4] = s.y; m.m[5] = u.y; m.m[6] = -f.y; m.m[7] = 0;
        m.m[8] = s.z; m.m[9] = u.z; m.m[10] = -f.z; m.m[11] = 0;
        m.m[12] = -(s.x * eye.x + s.y * eye.y + s.z * eye.z);
        m.m[13] = -(u.x * eye.x + u.y * eye.y + u.z * eye.z);
        m.m[14] = f.x * eye.x + f.y * eye.y + f.z * eye.z;
        m.m[15] = 1;
        return m;
    }

    Mat4 translate(const Mat4& m, const Vec3& v) {
        Mat4 result = m;
        result.m[12] = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12];
        result.m[13] = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13];
        result.m[14] = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
        result.m[15] = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15];
        return result;
    }

    Mat4 rotate(const Mat4& m, float angle, const Vec3& v) {
        float c = cosf(angle), s = sinf(angle);
        float x = v.x, y = v.y, z = v.z;
        float len = sqrtf(x * x + y * y + z * z);
        if (len != 0) { x /= len; y /= len; z /= len; }

        Mat4 rot;
        rot.m[0] = c + x * x * (1 - c);     rot.m[4] = x * y * (1 - c) - z * s;  rot.m[8] = x * z * (1 - c) + y * s;
        rot.m[1] = y * x * (1 - c) + z * s;  rot.m[5] = c + y * y * (1 - c);     rot.m[9] = y * z * (1 - c) - x * s;
        rot.m[2] = z * x * (1 - c) - y * s;  rot.m[6] = z * y * (1 - c) + x * s;  rot.m[10] = c + z * z * (1 - c);

        Mat4 result;
        mul(result.m, m.m, rot.m);
        return result;
    }

}  // namespace engine::math