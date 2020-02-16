#pragma once

#include <mak3do/game/PhysicsElement2D.h>
#include <box2D/box2D.h>

namespace mak3do {
class PhysicsElement2DImpl : public PhysicsElementImpl {
public:
    PhysicsElement2DImpl(PhysicsElement2D* parent);

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

    void impulse(const Vec3& impulse);
    void impulse(const Vec3& impulse, const Vec3& point);

    void force(const Vec3& force);
    void force(const Vec3& force, const Vec3& point);

    void body(b2World* body);
    void destroy_body(b2World* world);
    b2Body* body() const;

    void process_collision(PhysicsElementPtr other);

    PhysicsElement2D* parent() const;

private:
    std::vector<CollisionCallbackPtr> m_callbacks;

    PhysicsElement2D* m_parent;
    PhysicsWorldPtr m_parent_world;

    b2Body* m_body;
};
}
