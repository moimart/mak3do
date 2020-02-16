#include <mak3do/game/box2d/PhysicsWorld2DImpl.h>
#include <mak3do/game/box2d/PhysicsElement2DImpl.h>

namespace mak3do {

PhysicsWorld2DImpl::PhysicsWorld2DImpl(PhysicsWorld* parent)
: m_parent(parent)
{
    m_world = std::make_shared<b2World>(b2Vec2(0, 0));

    m_world->SetContactListener(this);
}

void PhysicsWorld2DImpl::add_element(PhysicsElementPtr element)
{
    PhysicsElement2DPtr element2D = std::dynamic_pointer_cast<PhysicsElement2D>(element);

    assert(element2D != PhysicsElement2DPtr());

    std::static_pointer_cast<PhysicsElement2DImpl>(element2D->m_pimpl)->body(m_world.get());

    element->world(std::static_pointer_cast<PhysicsWorld>(m_parent->shared_from_this()));
}

void PhysicsWorld2DImpl::remove_element(PhysicsElementPtr element)
{
    PhysicsElement2DPtr element2D = std::dynamic_pointer_cast<PhysicsElement2D>(element);

    assert(element2D != PhysicsElement2DPtr());

    std::static_pointer_cast<PhysicsElement2DImpl>(element2D->m_pimpl)->destroy_body(m_world.get());

    element->world(PhysicsWorldPtr());
}

void PhysicsWorld2DImpl::update(float dt)
{
    m_world->Step(dt, 10, 10);
}

void PhysicsWorld2DImpl::gravity(const Vec3& gravity)
{
    m_world->SetGravity(b2Vec2(gravity.x, gravity.y));
}

void PhysicsWorld2DImpl::BeginContact(b2Contact* contact)
{
    auto e1 = static_cast<PhysicsElement2DImpl*>(contact->GetFixtureA()->GetBody()->GetUserData());
    auto e2 = static_cast<PhysicsElement2DImpl*>(contact->GetFixtureB()->GetBody()->GetUserData());

    e1->process_collision(std::static_pointer_cast<PhysicsElement>(e2->parent()->shared_from_this()));
    e2->process_collision(std::static_pointer_cast<PhysicsElement>(e1->parent()->shared_from_this()));
}

void PhysicsWorld2DImpl::EndContact(b2Contact* contact)
{
}

void PhysicsWorld2DImpl::collided(CollisionCallbackPtr callback)
{
    m_collision_callbacks.push_back(callback);
}

void PhysicsWorld2DImpl::stop_collided(const std::string& id)
{
    std::remove_if(m_collision_callbacks.begin(),
                   m_collision_callbacks.end(),
                   [&](CollisionCallbackPtr& callback) {
       
        return callback->cb_id == id;
    });
}

void PhysicsWorld2DImpl::stop_collided(CollisionCallbackPtr callback)
{
    std::remove(m_collision_callbacks.begin(),
                m_collision_callbacks.end(),
                callback);
}

void PhysicsWorld2DImpl::cleanup_collided()
{
    m_collision_callbacks.clear();
}

}
