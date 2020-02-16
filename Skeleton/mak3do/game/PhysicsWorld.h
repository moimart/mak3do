#pragma once

#include "types.h"

namespace mak3do {
//subclass to PhysicsWorld2D and PhysicsWorld3D
class PhysicsWorldImpl : public std::enable_shared_from_this<PhysicsWorldImpl>  {
public:
    virtual ~PhysicsWorldImpl() {}

    virtual void add_element(PhysicsElementPtr element) = 0;
    virtual void remove_element(PhysicsElementPtr element) = 0;

    virtual void update(float dt) = 0;

    virtual void gravity(const Vec3& gravity) = 0;

    virtual void collided(CollisionCallbackPtr callback) = 0;
    virtual void stop_collided(const std::string& id) = 0;
    virtual void stop_collided(CollisionCallbackPtr callback) = 0;
    virtual void cleanup_collided() = 0;
};

class PhysicsWorld : public std::enable_shared_from_this<PhysicsWorld> {
public:
    enum class Type {
        _2D = 0,
        _3D
    };

    PhysicsWorld();
    PhysicsWorld(Type type);

    virtual ~PhysicsWorld() = default;

    virtual void add_element(PhysicsElementPtr element);
    virtual void remove_element(PhysicsElementPtr element);

    virtual void update(float dt);

    virtual void gravity(const Vec3& gravity);

    virtual void collided(CollisionCallbackPtr callback);
    virtual void stop_collided(const std::string& id);
    virtual void stop_collided(CollisionCallbackPtr callback);
    virtual void cleanup_collided();

protected:
    std::shared_ptr<PhysicsWorldImpl> m_pimpl;
};
}
