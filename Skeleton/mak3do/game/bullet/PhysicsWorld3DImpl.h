#pragma once

#include <mak3do/game/PhysicsWorld.h>
#include <btBulletDynamicsCommon.h>

namespace mak3do {
class PhysicsWorld3DImpl : public PhysicsWorldImpl {
public:
    PhysicsWorld3DImpl(PhysicsWorld* parent);

    void add_element(PhysicsElementPtr element);
    void remove_element(PhysicsElementPtr element);

    void gravity(const Vec3& gravity);

    void collided(CollisionCallbackPtr callback);
    void stop_collided(const std::string& id);
    void stop_collided(CollisionCallbackPtr callback);
    void cleanup_collided();

    void update(float dt);

private:
    PhysicsWorld* m_parent;
    std::shared_ptr<btDiscreteDynamicsWorld> m_world;
    std::shared_ptr<btDefaultCollisionConfiguration> m_collision_configuration;
    std::shared_ptr<btCollisionDispatcher> m_collision_dispatcher;
    std::shared_ptr<btSequentialImpulseConstraintSolver> m_constraint_solver;
    std::shared_ptr<btBroadphaseInterface> m_broadphase;

    std::vector<CollisionCallbackPtr> m_callbacks;
};
}
