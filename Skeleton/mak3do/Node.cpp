#include "Node.h"
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

std::vector<NodePtr> Node::getChildren() const
{
    return m_pimpl->getChildren();
}

void Node::setPosition(const Vec3& position)
{
    m_pimpl->setPosition(position);
}

Vec3 Node::getPosition() const
{
    return m_pimpl->getPosition();
}

}
