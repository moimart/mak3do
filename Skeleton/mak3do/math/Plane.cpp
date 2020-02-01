#include "Plane.h"
#include <math.h>

// From kazMath/Plane.[h/cpp]
// Copyright (c) 2008, Luke Benstead.
// All rights reserved.

using namespace mak3do;

void Plane::extractPlaneFromMat4(const Mat4& mat, Plane* plane, PlaneType type)
{
    Plane& p = *plane;

    float t = 1.0f;

    switch (type) {
    case PLANE_RIGHT:
        p.a = mat.m[3] - mat.m[0];
        p.b = mat.m[7] - mat.m[4];
        p.c = mat.m[11] - mat.m[8];
        p.d = mat.m[15] - mat.m[12];
        break;
    case PLANE_LEFT:
        p.a = mat.m[3] + mat.m[0];
        p.b = mat.m[7] + mat.m[4];
        p.c = mat.m[11] + mat.m[8];
        p.d = mat.m[15] + mat.m[12];
        break;
    case PLANE_BOTTOM:
        p.a = mat.m[3] + mat.m[1];
        p.b = mat.m[7] + mat.m[5];
        p.c = mat.m[11] + mat.m[9];
        p.d = mat.m[15] + mat.m[13];
        break;
    case PLANE_TOP:
        p.a = mat.m[3] - mat.m[1];
        p.b = mat.m[7] - mat.m[5];
        p.c = mat.m[11] - mat.m[9];
        p.d = mat.m[15] - mat.m[13];
        break;
    case PLANE_FAR:
        p.a = mat.m[3] - mat.m[2];
        p.b = mat.m[7] - mat.m[6];
        p.c = mat.m[11] - mat.m[10];
        p.d = mat.m[15] - mat.m[14];
        break;
    case PLANE_NEAR:
        p.a = mat.m[3] + mat.m[2];
        p.b = mat.m[7] + mat.m[6];
        p.c = mat.m[11] + mat.m[10];
        p.d = mat.m[15] + mat.m[14];
        break;
    default:
        //FIXME
        break;
    }

    t = sqrtf(p.a * p.a + p.b * p.b + p.c * p.c);
    p.a /= t;
    p.b /= t;
    p.c /= t;
    p.d /= t;
}

void Plane::createPlaneFromPointNormal(const Vec3& point, const Vec3& normal, Plane* plane)
{
    Plane& p = *plane;

    p.a = normal.x;
    p.b = normal.y;
    p.c = normal.z;
    p.d = -normal.dot(point);
}

void Plane::createPlaneFromPoints(const Vec3& p1, const Vec3& p2, const Vec3& p3, Plane* plane)
{
    Plane& p = *plane;

    Vec3 n, v1, v2;
    v1 = p2 - p1;
    v2 = p3 - p1;
    n = v1;
    n.cross(v2);
    n.normalize();
    n.scale(-1.0f);

    p.a = n.x;
    p.b = n.y;
    p.c = n.z;
    p.d = n.dot(p1);
}

Plane::Plane()
{
}

Plane::Plane(float _a, float _b, float _c, float _d)
    : a(_a)
    , b(_b)
    , c(_c)
    , d(_d)
{
}

float Plane::dot(const Vec4& vec)
{
    //a*x + b*y + c*z + d*w

    return (this->a * vec.x + this->b * vec.y + this->c * vec.z + this->d * vec.w);
}

float Plane::dotCoord(const Vec3& vec)
{
    return (this->a * vec.x + this->b * vec.y + this->c * vec.z + this->d);
}

float Plane::dotNormal(const Vec3& vec)
{
    return (this->a * vec.x + this->b * vec.y + this->c * vec.z);
}

const Vec3 Plane::intersectLine(const Vec3& p1, const Vec3& p2)
{
    return Vec3::ZERO;
}

void Plane::normalize()
{
}

void Plane::scale(float scale)
{
}

Plane::PointClassification Plane::classifyPoint(const Vec3& p)
{
    float distance = this->a * p.x + this->b * p.y + this->c * p.z + this->d;

    // Simply put if the dot product is greater than 0 then it is infront of it.
    // If it is less than 0 then it is behind it.  And if it is 0 then it is on it.
    if (distance > 0.001)
        return POINT_INFRONT_OF_PLANE;
    if (distance < -0.001)
        return POINT_BEHIND_PLANE;

    return POINT_ON_PLANE;
}
