#include <mak3do/game/box2d/PhysicsElement2DImpl.h>

using namespace mak3do;

PhysicsElement2DImpl::PhysicsElement2DImpl(PhysicsElement2D* parent)
: m_parent(parent)
, m_body(nullptr)
{
}

void PhysicsElement2DImpl::world(PhysicsWorldPtr world)
{
    m_parent_world = world;
}

PhysicsWorldPtr PhysicsElement2DImpl::world() const
{
    return m_parent_world;
}

void PhysicsElement2DImpl::position(const Vec3& position)
{
    if (m_body != nullptr) {
        m_body->SetTransform(b2Vec2(position.x, position.y), 0);
    }
}

Vec3 PhysicsElement2DImpl::position() const
{
    Vec3 position;
    if (m_body != nullptr) {
        auto pos = m_body->GetPosition();
        position = Vec3(pos.x, pos.y, 0);
    } else {
        position = Vec3::ZERO;
    }

    return position;
}

void PhysicsElement2DImpl::body(b2World* world)
{
    b2BodyDef body_def;

    body_def.type = b2BodyType::b2_dynamicBody;
    body_def.position = b2Vec2(0, 0);
    body_def.awake = true;

    m_body = world->CreateBody(&body_def);

    b2FixtureDef def;

    b2CircleShape circleShape;
    circleShape.m_radius = 4.0f;

    def.shape = &circleShape;
    def.density = 1;

    m_body->CreateFixture(&def);

    m_body->SetUserData(this);
}

void PhysicsElement2DImpl::destroy_body(b2World* world)
{
    world->DestroyBody(m_body);
    m_body = nullptr;
}

b2Body* PhysicsElement2DImpl::body() const
{
    return m_body;
}

PhysicsElement2D* PhysicsElement2DImpl::parent() const
{
    return m_parent;
}

void PhysicsElement2DImpl::collided(CollisionCallbackPtr callback)
{
    m_callbacks.push_back(callback);
}

void PhysicsElement2DImpl::stop_collided(CollisionCallbackPtr callback)
{
    std::remove(m_callbacks.begin(),m_callbacks.end(),callback);
}

void PhysicsElement2DImpl::cleanup_collided()
{
    m_callbacks.clear();
}

void PhysicsElement2DImpl::process_collision(PhysicsElementPtr other)
{
    auto this_object = std::static_pointer_cast<PhysicsElement2D>(m_parent->shared_from_this());
    for (auto& callback : m_callbacks) {
        callback->lambda(this_object, other);
    }
}

void PhysicsElement2DImpl::velocity(const Vec3& velocity)
{
    if (m_body != nullptr) {
        m_body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
    }
}

void PhysicsElement2DImpl::rotation(const Quaternion& rotation)
{
    //TODO: FIXME: Do this!
}

Quaternion PhysicsElement2DImpl::rotation() const
{
    Quaternion q;

    if (m_body != nullptr) {
        q.yaw_pitch_roll(0, 0, m_body->GetAngle());
    }

    return q;
}

void PhysicsElement2DImpl::impulse(const Vec3& impulse)
{
    if (m_body == nullptr) {
        return;
    }

    b2MassData data;
    m_body->GetMassData(&data);
    m_body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), data.center, true);
}

void PhysicsElement2DImpl::impulse(const Vec3& impulse, const Vec3& point)
{
    if (m_body == nullptr) {
        return;
    }

    m_body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
}

void PhysicsElement2DImpl::force(const Vec3& force)
{
    if (m_body == nullptr) {
        return;
    }

    m_body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void PhysicsElement2DImpl::force(const Vec3& force, const Vec3& point)
{
    if (m_body == nullptr) {
        return;
    }

    m_body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}
