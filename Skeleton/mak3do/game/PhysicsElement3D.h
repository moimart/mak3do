#pragma once

#include "PhysicsElement.h"

namespace mak3do {
class PhysicsElement3D : public PhysicsElement {
public:
    enum class Type {
        SPHERE,
        CAPSULE,
        BOX,
        CONE,
        CYLINDER,
        PLANE
    };

    PhysicsElement3D(Type type = Type::SPHERE,
        float radius = 0.025f,
        float height = 0,
        float mass = 1,
        const Vec3& extents = Vec3(1, 1, 1));

    void gravity(const Vec3& gravity);

private:
    friend class PhysicsWorld3DImpl;
};
}
