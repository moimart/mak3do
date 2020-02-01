#include "NodeImpl.h"
#include "../Node.h"
#import <SceneKit/SceneKit.h>

namespace mak3do {
NodeImpl::NodeImpl(Node* parent)
{
    SCNBox* box = [[SCNBox alloc] init];
    [box setWidth:1];
    [box setHeight:1];
    [box setLength:1];
    [box setChamferRadius: 0];
    box.materials.firstObject.diffuse.contents = [UIColor redColor];
    SCNNode* node = [[SCNNode alloc] init];
    [node setGeometry:box];
    [node setPosition:SCNVector3Make(0, 0, 0)];
    
    m_native = (void*)CFBridgingRetain(node);
}

NodeImpl::~NodeImpl()
{
    SCNNode* node = (SCNNode*)CFBridgingRelease(m_native);
    node = nil;
}

void NodeImpl::addChild(NodePtr node)
{
    SCNNode* native_node = (__bridge SCNNode*)m_native;
    SCNNode* child_native_node = (__bridge SCNNode*)node->m_pimpl->m_native;
    
    [native_node addChildNode:child_native_node];
    m_children.push_back(node);
}

std::vector<NodePtr> NodeImpl::getChildren() const
{
    return m_children;
}

void NodeImpl::setPosition(const Vec3& position)
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    [node setPosition:SCNVector3Make(position.x, position.y, position.z)];
}

Vec3 NodeImpl::getPosition() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    SCNVector3 native_position = [node position];
    return Vec3(native_position.x,native_position.y,native_position.z);
}

}
