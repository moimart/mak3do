#include "Node.h"
#include "Director.h"
#include "ActionRunner.h"
#ifdef M3D_PLAT_APPLE
#include "apple/NodeImpl.h"
#elif M3D_PLAT_ANDROID
#include "android/NodeImpl.h"
#endif

namespace mak3do {

Node::Node()
{
#ifdef M3D_PLAT_APPLE
    m_pimpl = std::make_shared<NodeImpl>(this);
#endif
}

void Node::addChild(NodePtr node)
{
    m_pimpl->addChild(node);
}

NodePtr Node::parent() const
{
    return m_pimpl->parent();
}

std::vector<NodePtr> Node::getChildren() const
{
    return m_pimpl->getChildren();
}

void Node::removeFromParent()
{
    m_pimpl->removeFromParent();
}

void Node::position(const Vec3& position)
{
    m_pimpl->position(position);
}

Vec3 Node::position() const
{
    return m_pimpl->position();
}

void Node::visible(bool visible)
{
    m_pimpl->visible(visible);
}

bool Node::visible() const
{
    return m_pimpl->visible();
}

void Node::scale(float scale)
{
    m_pimpl->scale(scale);
}

float Node::scale() const
{
    return m_pimpl->scale();
}

void Node::yaw(float yaw)
{
    m_pimpl->yaw(yaw);
}

float Node::yaw() const
{
    return m_pimpl->yaw();
}

void Node::pitch(float pitch)
{
    m_pimpl->pitch(pitch);
}

float Node::pitch() const
{
    return m_pimpl->pitch();
}

void Node::roll(float roll)
{
    m_pimpl->roll(roll);
}

float Node::roll() const
{
    return m_pimpl->roll();
}

void Node::rotation(const Quaternion& rotation)
{
    m_pimpl->rotation(rotation);
}

Quaternion Node::rotation() const
{
    return m_pimpl->rotation();
}

void Node::runAction(ActionPtr action)
{
    Director::get()->defaultActionRunner()->addAction(action,this->shared_from_this());
}

}
