#include "PhysicsWorld.h"
#include <mak3do/game/box2d/PhysicsWorld2DImpl.h>
#include <mak3do/game/bullet/PhysicsWorld3DImpl.h>

using namespace mak3do;

PhysicsWorld::PhysicsWorld()
: m_pimpl(std::make_shared<PhysicsWorld3DImpl>(this))
{
}

PhysicsWorld::PhysicsWorld(Type type)
{
    switch (type) {
    case Type::_2D:
        m_pimpl = std::make_shared<PhysicsWorld2DImpl>(this);
        break;

    case Type::_3D:
        m_pimpl = std::make_shared<PhysicsWorld3DImpl>(this);
        break;

    default:
        m_pimpl = std::make_shared<PhysicsWorld3DImpl>(this);
        break;
    }
}

void PhysicsWorld::add_element(PhysicsElementPtr element)
{
    m_pimpl->add_element(element);
}

void PhysicsWorld::remove_element(PhysicsElementPtr element)
{
    m_pimpl->remove_element(element);
}

void PhysicsWorld::gravity(const Vec3& gravity)
{
    m_pimpl->gravity(gravity);
}

void PhysicsWorld::collided(CollisionCallbackPtr callback)
{
    m_pimpl->collided(callback);
}

void PhysicsWorld::stop_collided(const std::string& id)
{
    m_pimpl->stop_collided(id);
}

void PhysicsWorld::stop_collided(CollisionCallbackPtr callback)
{
    m_pimpl->stop_collided(callback);
}

void PhysicsWorld::cleanup_collided()
{
    m_pimpl->cleanup_collided();
}

void PhysicsWorld::update(float dt)
{
    m_pimpl->update(dt);
}
