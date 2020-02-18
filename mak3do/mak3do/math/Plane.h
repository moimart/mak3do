#pragma once

#include "macros.h"
#include "Mat4.h"
#include "Vec3.h"
#include "Vec4.h"

// From kazMath/Plane.[h/cpp]
// Copyright (c) 2008, Luke Benstead.
// All rights reserved.

namespace mak3do {

class Plane {
public:
    enum PlaneType {
        PLANE_LEFT = 0,
        PLANE_RIGHT,
        PLANE_BOTTOM,
        PLANE_TOP,
        PLANE_NEAR,
        PLANE_FAR
    };

    enum PointClassification {
        POINT_INFRONT_OF_PLANE = 0,
        POINT_BEHIND_PLANE,
        POINT_ON_PLANE
    };

    static void extractPlaneFromMat4(const Mat4& mat, Plane* plane, PlaneType type);
    static void createPlaneFromPointNormal(const Vec3& point, const Vec3& normal, Plane* p);
    static void createPlaneFromPoints(const Vec3& p1, const Vec3& p2, const Vec3& p3, Plane* p);

    Plane();
    Plane(float _a, float _b, float _c, float _d);

    float a, b, c, d;

    float dot(const Vec4& vec);
    float dotCoord(const Vec3& vec);
    float dotNormal(const Vec3& vec);

    const Vec3 intersectLine(const Vec3& p1, const Vec3& p2);
    void normalize();
    void scale(float scale);
    PointClassification classifyPoint(const Vec3& p);
};

}
