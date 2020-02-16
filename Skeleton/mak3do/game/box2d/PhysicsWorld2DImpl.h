#pragma once

#include <mak3do/game/PhysicsWorld.h>
#include <Box2D/Box2D.h>

namespace mak3do {
class PhysicsWorld2DImpl : public PhysicsWorldImpl, public b2ContactListener {
public:
    PhysicsWorld2DImpl(PhysicsWorld* parent);

    void add_element(PhysicsElementPtr element);
    void remove_element(PhysicsElementPtr element);

    void gravity(const Vec3& gravity);

    void update(float dt);

    void collided(CollisionCallbackPtr callback);
    void stop_collided(const std::string& id);
    void stop_collided(CollisionCallbackPtr callback);
    void cleanup_collided();

    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);

private:
    PhysicsWorld* m_parent;
    std::shared_ptr<b2World> m_world;

    std::vector<CollisionCallbackPtr> m_collision_callbacks;
};
}
