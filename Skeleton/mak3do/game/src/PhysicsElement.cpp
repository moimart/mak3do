#include <mak3do/game/PhysicsElement.h>
#include <mak3do/game/PhysicsElement2D.h>
#include <mak3do/game/PhysicsElement3D.h>

namespace mak3do {

void PhysicsElement::world(PhysicsWorldPtr world)
{
    m_pimpl->world(world);
}

PhysicsWorldPtr PhysicsElement::world() const
{
    return m_pimpl->world();
}

void PhysicsElement::position(const Vec3& position)
{
    m_pimpl->position(position);
}

Vec3 PhysicsElement::position() const
{
    return m_pimpl->position();
}

void PhysicsElement::collided(CollisionCallbackPtr callback)
{
    m_pimpl->collided(callback);
}

void PhysicsElement::stop_collided(CollisionCallbackPtr callback)
{
    m_pimpl->stop_collided(callback);
}

void PhysicsElement::cleanup_collided()
{
    m_pimpl->cleanup_collided();
}

void PhysicsElement::velocity(const Vec3& velocity)
{
    m_pimpl->velocity(velocity);
}

void PhysicsElement::impulse(const Vec3& impulse)
{
    m_pimpl->impulse(impulse);
}

void PhysicsElement::impulse(const Vec3& impulse, const Vec3& point)
{
    m_pimpl->impulse(impulse, point);
}

void PhysicsElement::force(const Vec3& force)
{
    m_pimpl->force(force);
}

void PhysicsElement::force(const Vec3& force, const Vec3& point)
{
    m_pimpl->force(force, point);
}

void PhysicsElement::rotation(const Quaternion& rotation)
{
    m_pimpl->rotation(rotation);
}

Quaternion PhysicsElement::rotation() const
{
    return m_pimpl->rotation();
}

////// FACTORY

static inline PhysicsElementPtr process_2d_shape(mak3do::ElementFactory::ElementDef2D* element)
{
    return mak3do::create<PhysicsElement2D>();
}

static inline PhysicsElementPtr process_3d_shape(mak3do::ElementFactory::ElementDef3D* element)
{
    using namespace ElementFactory;

    switch (element->shape) {
    case ElementDef3D::shape_type::SPHERE:
        return mak3do::create<PhysicsElement3D>();
        break;

    default:
        return mak3do::create<PhysicsElement3D>();
        break;
    }
}

PhysicsElementPtr ElementFactory::make_basic_element(const ElementDef& element)
{
    ElementDef local_copy = element;

    switch (element.type) {
    case ElementDef::physics_type::_2D:
        return process_2d_shape(static_cast<ElementDef2D*>(&local_copy));
        break;
    case ElementDef::physics_type::_3D:
        return process_3d_shape(static_cast<ElementDef3D*>(&local_copy));
        break;
    default:
        break;
    }

    return nullptr;
}

}
