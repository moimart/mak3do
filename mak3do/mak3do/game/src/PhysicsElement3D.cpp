#include <mak3do/game/PhysicsElement3D.h>
#include <mak3do/game/bullet/PhysicsElement3DImpl.h>

namespace mak3do {

PhysicsElement3D::PhysicsElement3D(Type type,
    float radius,
    float height,
    float mass,
    const Vec3& extents)
{
    m_pimpl = std::make_shared<PhysicsElement3DImpl>(this, type, radius, height, mass, extents);
}

void PhysicsElement3D::gravity(const Vec3& gravity)
{
    std::static_pointer_cast<PhysicsElement3DImpl>(m_pimpl)->gravity(gravity);
}

}
