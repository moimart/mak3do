#include <mak3do/game/bullet/PhysicsElement3DImpl.h>

using namespace mak3do;

namespace mak3do {
struct body_elements {
    std::shared_ptr<btCollisionShape> shape;
    std::shared_ptr<btDefaultMotionState> motion_state;
    std::shared_ptr<btRigidBody> body;
};
}

PhysicsElement3DImpl::PhysicsElement3DImpl(PhysicsElement3D* parent,
    PhysicsElement3D::Type type,
    float radius,
    float height,
    float mass,
    const Vec3& extents)
: m_parent(parent)
, m_body_elements(std::make_shared<body_elements>())
{
    switch (type) {
    default:
    case PhysicsElement3D::Type::SPHERE:
        m_body_elements->shape = std::make_shared<btSphereShape>((btScalar)radius);
        break;
    case PhysicsElement3D::Type::CAPSULE:
        m_body_elements->shape = std::make_shared<btCapsuleShape>((btScalar)radius, (btScalar)height);
        break;
    case PhysicsElement3D::Type::BOX:
        m_body_elements->shape = std::make_shared<btBoxShape>(btVector3(extents.x, extents.y, extents.z));
        break;
    case PhysicsElement3D::Type::CONE:
        m_body_elements->shape = std::make_shared<btConeShape>((btScalar)radius, (btScalar)height);
        break;
    case PhysicsElement3D::Type::CYLINDER:
        m_body_elements->shape = std::make_shared<btCylinderShape>(btVector3(extents.x, extents.y, extents.z));
        break;
    case PhysicsElement3D::Type::PLANE:
        m_body_elements->shape = std::make_shared<btStaticPlaneShape>(btVector3(0, 1, 0), radius); //FIXME: Pass this!
        break;
    }

    btVector3 center_of_mass(0, 0, 0);
    if (type == PhysicsElement3D::Type::PLANE)
        center_of_mass = btVector3(0, -1, 0);

    m_body_elements->motion_state = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), center_of_mass));

    btScalar _mass = mass;

    if (type == PhysicsElement3D::Type::PLANE)
        _mass = 0;

    btVector3 fallInertia(0, 0, 0);
    m_body_elements->shape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(_mass,
        m_body_elements->motion_state.get(),
        m_body_elements->shape.get(),
        fallInertia);

    m_body_elements->body = std::make_shared<btRigidBody>(fallRigidBodyCI);

    m_body = m_body_elements->body.get();

    m_body->setUserPointer(this);
}

void PhysicsElement3DImpl::gravity(const Vec3& gravity)
{
    m_body->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void PhysicsElement3DImpl::world(PhysicsWorldPtr world)
{
    m_parent_world = world;
}

PhysicsWorldPtr PhysicsElement3DImpl::world() const
{
    return m_parent_world;
}

void PhysicsElement3DImpl::position(const Vec3& position)
{
    m_body->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));
}

Vec3 PhysicsElement3DImpl::position() const
{
    Vec3 position;

    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);

    position = Vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

    return position;
}

PhysicsElement3D* PhysicsElement3DImpl::parent() const
{
    return m_parent;
}

void PhysicsElement3DImpl::collided(CollisionCallbackPtr callback)
{
    m_callbacks.push_back(callback);
}

void PhysicsElement3DImpl::stop_collided(CollisionCallbackPtr callback)
{
    m_callbacks.erase(std::remove(m_callbacks.begin(),
                                  m_callbacks.end(),
                                  callback)
    ,m_callbacks.end());
}

void PhysicsElement3DImpl::cleanup_collided()
{
    m_callbacks.clear();
}

void PhysicsElement3DImpl::process_collision(PhysicsElementPtr other)
{
    auto this_object = std::static_pointer_cast<PhysicsElement3D>(m_parent->shared_from_this());
    for (auto& callback : m_callbacks) {
        callback->lambda(this_object, other);
    }
}

void PhysicsElement3DImpl::velocity(const Vec3& velocity)
{
    m_body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

void PhysicsElement3DImpl::rotation(const Quaternion& rotation)
{
    //FIXME: TODO: Do this!!!
}

Quaternion PhysicsElement3DImpl::rotation() const
{
    Vec3 rotation;

    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);

    auto quat = trans.getRotation();
    Quaternion q(quat.getX(), quat.getY(), quat.getZ(), quat.getW());

    return q;
}

btRigidBody* PhysicsElement3DImpl::body() const
{
    return m_body;
}

void PhysicsElement3DImpl::impulse(const Vec3& impulse)
{
    if (m_body == nullptr) {
        return;
    }

    m_body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}

void PhysicsElement3DImpl::impulse(const Vec3& impulse, const Vec3& point)
{
    if (m_body == nullptr) {
        return;
    }

    m_body->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(point.x, point.y, point.z));
}

void PhysicsElement3DImpl::force(const Vec3& force)
{
    if (m_body == nullptr) {
        return;
    }

    m_body->applyCentralForce(btVector3(force.x, force.y, force.z));
}

void PhysicsElement3DImpl::force(const Vec3& force, const Vec3& point)
{
    if (m_body == nullptr) {
        return;
    }

    m_body->applyForce(btVector3(force.x, force.y, force.z), btVector3(point.x, point.y, point.z));
}
