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
    //m_pimpl = std::make_shared<NodePriv>(this);
#endif
}

void Node::addChild(NodePtr node)
{
    m_children.push_back(node);
}

std::vector<NodePtr> Node::getChildren() const
{
    return m_children;
}

}
