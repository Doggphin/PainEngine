// Copyright (C) 2022 Martin Weigel <mail@MartinWeigel.com>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

/**
 * @file    Quaternion.c
 * @brief   A basic quaternion library written in C
 * @date    2022-05-16
 */
#include "Quaternion.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif

void Quaternion_set(float w, float v1, float v2, float v3, Quaternion* output)
{
    assert(output != NULL);
    output->w = w;
    output->v[0] = v1;
    output->v[1] = v2;
    output->v[2] = v3;
}

void Quaternion_setIdentity(Quaternion* q)
{
    assert(q != NULL);
    Quaternion_set(1, 0, 0, 0, q);
}

void Quaternion_copy(Quaternion* q, Quaternion* output)
{
    Quaternion_set(q->w, q->v[0], q->v[1], q->v[2], output);
}

bool Quaternion_equal(Quaternion* q1, Quaternion* q2)
{
    bool equalW  = fabs(q1->w - q2->w) <= QUATERNION_EPS;
    bool equalV0 = fabs(q1->v[0] - q2->v[0]) <= QUATERNION_EPS;
    bool equalV1 = fabs(q1->v[1] - q2->v[1]) <= QUATERNION_EPS;
    bool equalV2 = fabs(q1->v[2] - q2->v[2]) <= QUATERNION_EPS;
    return equalW && equalV0 && equalV1 && equalV2;
}

void Quaternion_fprint(FILE* file, Quaternion* q)
{
    fprintf(file, "(%.3f, %.3f, %.3f, %.3f)",
        q->w, q->v[0], q->v[1], q->v[2]);
}


void Quaternion_fromAxisAngle(Vector3* axis, float angle, Quaternion* output)
{
    assert(output != NULL);
    // Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
    output->w = cos(angle / 2.0);
    float c = sin(angle / 2.0);
    output->v[0] = c * axis->x;
    output->v[1] = c * axis->y;
    output->v[2] = c * axis->z;
}

float Quaternion_toAxisAngle(Quaternion* q, Vector3* output)
{
    assert(output != NULL);
    // Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/
    float angle = 2.0 * acos(q->w);
    float divider = sqrt(1.0 - q->w * q->w);

    if(divider != 0.0) {
        // Calculate the axis
        output->x = q->v[0] / divider;
        output->y = q->v[1] / divider;
        output->z = q->v[2] / divider;
    } else {
        // Arbitrary normalized axis
        output->x = 1;
        output->y = 0;
        output->z = 0;
    }
    return angle;
}

void Quaternion_fromXRotation(float angle, Quaternion* output)
{
    assert(output != NULL);
    Vector3 axis;
    Vector3_set(1, 0, 0, &axis);
    Quaternion_fromAxisAngle(&axis, angle, output);
}

void Quaternion_fromYRotation(float angle, Quaternion* output)
{
    assert(output != NULL);
    Vector3 axis;
    Vector3_set(0, 1, 0, &axis);
    Quaternion_fromAxisAngle(&axis, angle, output);
}

void Quaternion_fromZRotation(float angle, Quaternion* output)
{
    assert(output != NULL);
    Vector3 axis;
    Vector3_set(0, 0, 1, &axis);
    Quaternion_fromAxisAngle(&axis, angle, output);
}

void Quaternion_fromEulerZYX(Vector3* eulerXYZ, Quaternion* output)
{
    assert(output != NULL);
    // Based on https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
    float cy = cos(eulerXYZ->z * 0.5);
    float sy = sin(eulerXYZ->z * 0.5);
    float cr = cos(eulerXYZ->x * 0.5);
    float sr = sin(eulerXYZ->x * 0.5);
    float cp = cos(eulerXYZ->y * 0.5);
    float sp = sin(eulerXYZ->y * 0.5);

    output->w = cy * cr * cp + sy * sr * sp;
    output->v[0] = cy * sr * cp - sy * cr * sp;
    output->v[1] = cy * cr * sp + sy * sr * cp;
    output->v[2] = sy * cr * cp - cy * sr * sp;
}

void Quaternion_toEulerZYX(Quaternion* q, Vector3* output)
{
    assert(output != NULL);

    // Roll (x-axis rotation)
    float sinr_cosp = +2.0 * (q->w * q->v[0] + q->v[1] * q->v[2]);
    float cosr_cosp = +1.0 - 2.0 * (q->v[0] * q->v[0] + q->v[1] * q->v[1]);
    output->x = atan2(sinr_cosp, cosr_cosp);

    // Pitch (y-axis rotation)
    float sinp = +2.0 * (q->w * q->v[1] - q->v[2] * q->v[0]);
    if (fabs(sinp) >= 1)
        output->y = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        output->y = asin(sinp);

    // Yaw (z-axis rotation)
    float siny_cosp = +2.0 * (q->w * q->v[2] + q->v[0] * q->v[1]);
    float cosy_cosp = +1.0 - 2.0 * (q->v[1] * q->v[1] + q->v[2] * q->v[2]);
    output->z = atan2(siny_cosp, cosy_cosp);
}

void Quaternion_conjugate(Quaternion* q, Quaternion* output)
{
    assert(output != NULL);
    output->w = q->w;
    output->v[0] = -q->v[0];
    output->v[1] = -q->v[1];
    output->v[2] = -q->v[2];
}

float Quaternion_norm(Quaternion* q)
{
    assert(q != NULL);
    return sqrt(q->w*q->w + q->v[0]*q->v[0] + q->v[1]*q->v[1] + q->v[2]*q->v[2]);
}

void Quaternion_normalize(Quaternion* q, Quaternion* output)
{
    assert(output != NULL);
    float len = Quaternion_norm(q);
    Quaternion_set(
        q->w / len,
        q->v[0] / len,
        q->v[1] / len,
        q->v[2] / len,
        output);
}

void Quaternion_multiply(Quaternion* q1, Quaternion* q2, Quaternion* output)
{
    assert(output != NULL);
    Quaternion result;

    /*
    Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/arithmetic/index.htm
             a*e - b*f - c*g - d*h
        + i (b*e + a*f + c*h- d*g)
        + j (a*g - b*h + c*e + d*f)
        + k (a*h + b*g - c*f + d*e)
    */
    result.w =    q1->w   *q2->w    - q1->v[0]*q2->v[0] - q1->v[1]*q2->v[1] - q1->v[2]*q2->v[2];
    result.v[0] = q1->v[0]*q2->w    + q1->w   *q2->v[0] + q1->v[1]*q2->v[2] - q1->v[2]*q2->v[1];
    result.v[1] = q1->w   *q2->v[1] - q1->v[0]*q2->v[2] + q1->v[1]*q2->w    + q1->v[2]*q2->v[0];
    result.v[2] = q1->w   *q2->v[2] + q1->v[0]*q2->v[1] - q1->v[1]*q2->v[0] + q1->v[2]*q2->w   ;

    *output = result;
}

void Quaternion_rotate(Quaternion* q, Vector3* v, Vector3* output)
{
    assert(output != NULL);
    /*
    // https://stackoverflow.com/questions/22497093/faster-quaternion-vector-multiplication-doesnt-work

    float tx = 2 * (q->v[1] * v->z - q->v[2] * v->y);
    float ty = 2 * (q->v[2] * v->x - q->v[0] * v->z);
    float tz = 2 * (q->v[0] * v->y - q->v[1] * v->x);

    output->x = v->x + q->w * tx + (q->v[1] * tz - q->v[2] * ty);
    output->y = v->y + q->w * ty + (q->v[2] * tx - q->v[0] * tz);
    output->y = v->z + q->w * tz + (q->v[0] * ty - q->v[1] * tx);
    */
    
    float ww = q->w * q->w;
    float xx = q->v[0] * q->v[0];
    float yy = q->v[1] * q->v[1];
    float zz = q->v[2] * q->v[2];
    float wx = q->w * q->v[0];
    float wy = q->w * q->v[1];
    float wz = q->w * q->v[2];
    float xy = q->v[0] * q->v[1];
    float xz = q->v[0] * q->v[2];
    float yz = q->v[1] * q->v[2];

    // Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
    // p2.x = w*w*p1.x + 2*y*w*p1.z - 2*z*w*p1.y + x*x*p1.x + 2*y*x*p1.y + 2*z*x*p1.z - z*z*p1.x - y*y*p1.x;
    // p2.y = 2*x*y*p1.x + y*y*p1.y + 2*z*y*p1.z + 2*w*z*p1.x - z*z*p1.y + w*w*p1.y - 2*x*w*p1.z - x*x*p1.y;
    // p2.z = 2*x*z*p1.x + 2*y*z*p1.y + z*z*p1.z - 2*w*y*p1.x - y*y*p1.z + 2*w*x*p1.y - x*x*p1.z + w*w*p1.z;

    output->x = ww*v->x + 2*wy*v->z - 2*wz*v->y +
                xx*v->x + 2*xy*v->y + 2*xz*v->z -
                zz*v->x - yy*v->x;
    output->y = 2*xy*v->x + yy*v->y + 2*yz*v->z +
                2*wz*v->x - zz*v->y + ww*v->y -
                2*wx*v->z - xx*v->y;
    output->z = 2*xz*v->x + 2*yz*v->y + zz*v->z -
                2*wy*v->x - yy*v->z + 2*wx*v->y -
                xx*v->z + ww*v->z;
}

Vector3* Quaternion_createRotated(Quaternion* q, Vector3* v) {
    Vector3* ret = malloc(sizeof(Vector3));
    Quaternion_rotate(q, v, ret);
    return ret;
}

void Quaternion_slerp(Quaternion* q1, Quaternion* q2, float t, Quaternion* output)
{
    Quaternion result;

    // Based on http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
    float cosHalfTheta = q1->w*q2->w + q1->v[0]*q2->v[0] + q1->v[1]*q2->v[1] + q1->v[2]*q2->v[2];

    // if q1=q2 or qa=-q2 then theta = 0 and we can return qa
    if (fabs(cosHalfTheta) >= 1.0) {
        Quaternion_copy(q1, output);
        return;
    }

    float halfTheta = acos(cosHalfTheta);
    float sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
    // If theta = 180 degrees then result is not fully defined
    // We could rotate around any axis normal to q1 or q2
    if (fabs(sinHalfTheta) < QUATERNION_EPS) {
        result.w = (q1->w * 0.5 + q2->w * 0.5);
        result.v[0] = (q1->v[0] * 0.5 + q2->v[0] * 0.5);
        result.v[1] = (q1->v[1] * 0.5 + q2->v[1] * 0.5);
        result.v[2] = (q1->v[2] * 0.5 + q2->v[2] * 0.5);
    } else {
        // Default quaternion calculation
        float ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
        float ratioB = sin(t * halfTheta) / sinHalfTheta;
        result.w = (q1->w * ratioA + q2->w * ratioB);
        result.v[0] = (q1->v[0] * ratioA + q2->v[0] * ratioB);
        result.v[1] = (q1->v[1] * ratioA + q2->v[1] * ratioB);
        result.v[2] = (q1->v[2] * ratioA + q2->v[2] * ratioB);
    }
    *output = result;
}


void Quaternion_fromToRotation(Vector3* from, Vector3* to, Quaternion* output) {
    /*
    // https://arrowinmyknee.com/2021/02/10/how-to-get-rotation-from-two-vectors/
    float norm_u_norm_v = sqrt(Vector3_sqrmagnitude(from) * Vector3_sqrmagnitude(to));
    float cos_theta = Vector3_dot(from, to) / norm_u_norm_v;
    float half_cos = sqrt(0.5f * (1 + cos_theta));
    struct Vector3 w;
    Vector3_cross(from, to, &w);
    Vector3_div(&w, (norm_u_norm_v * 2 * half_cos), &w);
    Quaternion_set(half_cos, w.x, w.y, w.z, output);
    // https://stackoverflow.com/a/11741520
    */
    
    float k_cos_theta = Vector3_dot(from, to);
    float k = sqrt(Vector3_sqrmagnitude(from) * Vector3_sqrmagnitude(to));
    Vector3 cache;
    if (k_cos_theta / k == -1) {
        Vector3_orthogonal(from, &cache);
        Vector3_normalize(&cache, &cache);
        Quaternion_set(0, cache.x, cache.y, cache.z, output);
    }
    else {
        Vector3_cross(from, to, &cache);
        Quaternion_set(k_cos_theta + k, cache.x, cache.y, cache.z, output);
        Quaternion_normalize(output, output);
    }
    
}

// https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
float* Quaternion_toMat4(Quaternion* q) {

    float* buf = malloc(sizeof(float) * 16);

    float x = -(q->v[0]);
    float y = -(q->v[1]);
    float z = -(q->v[2]);
    float w = q->w;
    buf[0] = 1 - 2*y*y - 2*z*z;
    buf[1] = 2*x*y - 2*z*w;
    buf[2] = 2*x*z + 2*y*w;
    buf[3] = 0;

    buf[4] = 2*x*y + 2*z*w;
    buf[5] = 1 - 2*x*x - 2*z*z;
    buf[6] = 2*y*z - 2*x*w;
    buf[7] = 0;

    buf[8] = 2*x*z - 2*y*w;
    buf[9] = 2*y*z + 2*x*w;
    buf[10] = 1 - 2*x*x - 2*y*y;
    buf[11] = 0;

    buf[12] = 0;
    buf[13] = 0;
    buf[14] = 0;
    buf[15] = 1;

    /*
    float ret[] = {
        1 - 2*(y*y - z*z),      2*(x*y - z*w),          2*(x*z + y*w),      0,
        2*(x*y) + 2*(z*w),      1 - 2*(x*x - z*z),      2*(y*z - x*w),      0,
        2*(x*z - y*w),          2*(y*z + x*w),          1 - 2*(x*x - y*y),  0,
        0,                      0,                      0,                  1
    };
    */
    //printf("The first row SHOULD be %.6f %.6f %.6f %.6f \n", ret[0], ret[1], ret[2], ret[3]);
    //buf = ret;
    return buf;
}

void Quaternion_print(Quaternion* q) {
    printf("(%.6f, %.6f, %.6f, %.6f)\n", q->w, q->v[0], q->v[1], q->v[2]);
}