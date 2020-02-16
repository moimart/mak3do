#include <mak3do/scenegraph/Node.h>
#include <mak3do/scenegraph/Director.h>
#include <mak3do/scenegraph/ActionRunner.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/NodeImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/NodeImpl.h>
#endif

namespace mak3do {

Node::Node()
{
    m_pimpl = std::make_shared<NodeImpl>(this);
}

void Node::add_node(NodePtr node)
{
    m_pimpl->addChild(node);
}

NodePtr Node::parent() const
{
    return m_pimpl->parent();
}

std::vector<NodePtr> Node::nodes() const
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

void Node::name(const std::string& name)
{
    m_pimpl->name(name);
}

std::string Node::name() const
{
    return m_pimpl->name();
}

void Node::action(ActionPtr action)
{
    Director::get()->defaultActionRunner()->add_action(action,this->shared_from_this());
}

void Node::stop_all_actions()
{
    Director::get()->defaultActionRunner()->remove_target(this->shared_from_this());
}

void Node::geometry(GeometryPtr geometry)
{
    m_pimpl->geometry(geometry);
}

GeometryPtr Node::geometry() const
{
    return m_pimpl->geometry();
}


}
