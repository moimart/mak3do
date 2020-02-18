#pragma once

#include "../PhysicsElement3D.h"
#include <btBulletDynamicsCommon.h>

namespace mak3do {
struct body_elements;

class PhysicsElement3DImpl : public PhysicsElementImpl {
public:
    PhysicsElement3DImpl(PhysicsElement3D* parent,
        PhysicsElement3D::Type type = PhysicsElement3D::Type::SPHERE,
        float radius = 0.025f,
        float height = 0,
        float mass = 1,
        const Vec3& extents = Vec3::ZERO);

    void world(PhysicsWorldPtr world);
    PhysicsWorldPtr world() const;

    void position(const Vec3& position);
    Vec3 position() const;

    void rotation(const Quaternion& rotation);
    Quaternion rotation() const;

    void collided(CollisionCallbackPtr callback);
    void stop_collided(CollisionCallbackPtr callback);
    void cleanup_collided();

    void velocity(const Vec3& velocity);
    void gravity(const Vec3& gravity);

    void impulse(const Vec3& impulse);
    void impulse(const Vec3& impulse, const Vec3& point);

    void force(const Vec3& force);
    void force(const Vec3& force, const Vec3& point);

    void process_collision(PhysicsElementPtr other);

    btRigidBody* body() const;

    PhysicsElement3D* parent() const;

private:
    std::vector<CollisionCallbackPtr> m_callbacks;

    PhysicsElement3D* m_parent;
    PhysicsWorldPtr m_parent_world;

    btRigidBody* m_body;
    std::shared_ptr<body_elements> m_body_elements;
};
}
