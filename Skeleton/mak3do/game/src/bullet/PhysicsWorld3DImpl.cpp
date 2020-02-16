#include <mak3do/game/bullet/PhysicsWorld3DImpl.h>
#include <mak3do/game/bullet/PhysicsElement3DImpl.h>

namespace mak3do {

PhysicsWorld3DImpl::PhysicsWorld3DImpl(PhysicsWorld* parent)
: m_parent(parent)
{
    m_broadphase = std::make_shared<btDbvtBroadphase>();
    m_collision_configuration = std::make_shared<btDefaultCollisionConfiguration>();
    m_collision_dispatcher = std::make_shared<btCollisionDispatcher>(m_collision_configuration.get());
    m_constraint_solver = std::make_shared<btSequentialImpulseConstraintSolver>();

    m_world = std::make_shared<btDiscreteDynamicsWorld>(m_collision_dispatcher.get(),
        m_broadphase.get(),
        m_constraint_solver.get(),
        m_collision_configuration.get());

    m_world->setGravity(btVector3(0, 0, 0));
}

void PhysicsWorld3DImpl::add_element(PhysicsElementPtr element)
{
    PhysicsElement3DPtr element3d = std::dynamic_pointer_cast<PhysicsElement3D>(element);

    assert(element3d != PhysicsElement3DPtr());

    auto body = std::static_pointer_cast<PhysicsElement3DImpl>(element3d->m_pimpl)->body();

    if (body != nullptr)
        m_world->addRigidBody(body);

    element->world(std::static_pointer_cast<PhysicsWorld>(m_parent->shared_from_this()));
}

void PhysicsWorld3DImpl::remove_element(PhysicsElementPtr element)
{
    PhysicsElement3DPtr element3d = std::dynamic_pointer_cast<PhysicsElement3D>(element);

    assert(element3d != PhysicsElement3DPtr());

    auto body = std::static_pointer_cast<PhysicsElement3DImpl>(element3d->m_pimpl)->body();

    if (body != nullptr) {
        m_world->removeRigidBody(body);
    }

    element->world(PhysicsWorldPtr());
}

void PhysicsWorld3DImpl::update(float dt)
{
    m_world->stepSimulation(1 / 60.f, 10);

    int numManifolds = m_world->getDispatcher()->getNumManifolds();

    for (int i = 0; i < numManifolds; i++) {
        btPersistentManifold* contactManifold = m_world->getDispatcher()->getManifoldByIndexInternal(i);

        auto obA = contactManifold->getBody0();
        auto obB = contactManifold->getBody1();

        auto pe_ptrA = static_cast<PhysicsElement3DImpl*>(obA->getUserPointer());
        auto pe_ptrB = static_cast<PhysicsElement3DImpl*>(obB->getUserPointer());

        PhysicsElementPtr elementA = std::static_pointer_cast<PhysicsElement>(pe_ptrA->parent()->shared_from_this());
        PhysicsElementPtr elementB = std::static_pointer_cast<PhysicsElement>(pe_ptrB->parent()->shared_from_this());

        pe_ptrA->process_collision(elementB);
        pe_ptrB->process_collision(elementA);

        for (auto& callback : m_callbacks) {
            callback->lambda(elementA, elementB);
        }
        /*
        int numContacts = contactManifold->getNumContacts();
        for (int j=0;j<numContacts;j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance()<0.f)
            {
                const btVector3& ptA = pt.getPositionWorldOnA();
                const btVector3& ptB = pt.getPositionWorldOnB();
                const btVector3& normalOnB = pt.m_normalWorldOnB;
            }
        }*/
    }
}

void PhysicsWorld3DImpl::gravity(const Vec3& gravity)
{
    m_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void PhysicsWorld3DImpl::collided(CollisionCallbackPtr callback)
{
    m_callbacks.push_back(callback);
}

void PhysicsWorld3DImpl::stop_collided(const std::string& id)
{
    std::remove_if(m_callbacks.begin(),
                   m_callbacks.end(),
                   [&](CollisionCallbackPtr& callback) {
       
        return callback->cb_id == id;
    });
}

void PhysicsWorld3DImpl::stop_collided(CollisionCallbackPtr callback)
{
    std::remove(m_callbacks.begin(),
                m_callbacks.end(),
                callback);
}

void PhysicsWorld3DImpl::cleanup_collided()
{
    m_callbacks.clear();
}

}
