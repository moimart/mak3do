#include "NodeImpl.h"
#include "../Node.h"
#import <SceneKit/SceneKit.h>

namespace mak3do {
NodeImpl::NodeImpl(Node* parent)
: m_abstract(parent)
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
    
    node->m_pimpl->m_parent_node = m_abstract->shared_from_this();
}

std::vector<NodePtr> NodeImpl::getChildren() const
{
    return m_children;
}

NodePtr NodeImpl::parent() const
{
    return m_parent_node;
}

void NodeImpl::removeFromParent()
{
    SCNNode* native_node = (__bridge SCNNode*)m_native;
    [native_node removeFromParentNode];
    
    std::remove(m_abstract->m_pimpl->m_children.begin(), m_abstract->m_pimpl->m_children.begin(), m_abstract->shared_from_this());
}

void NodeImpl::position(const Vec3& position)
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    [node setPosition:SCNVector3Make(position.x, position.y, position.z)];
}

Vec3 NodeImpl::position() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    SCNVector3 native_position = [node position];
    return Vec3(native_position.x,native_position.y,native_position.z);
}

void NodeImpl::visible(bool visible)
{
    SCNNode* node = (__bridge SCNNode*)m_native;
}

bool NodeImpl::visible() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return true;
}

void NodeImpl::scale(float scale)
{
    SCNNode* node = (__bridge SCNNode*)m_native;
}

float NodeImpl::scale() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return 1;
}

inline void setYPR(SCNNode* node, const Vec3& ypr)
{
    Quaternion quat;
    quat.setYawPitchRoll(ypr);
    
    [node setRotation:SCNVector4Make(quat.x, quat.y, quat.z, quat.z)];
}

void NodeImpl::yaw(float yaw)
{
    m_ypr.x = yaw;
    setYPR((__bridge SCNNode*)m_native, m_ypr);
}

float NodeImpl::yaw() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return m_ypr.x;
}

void NodeImpl::pitch(float pitch)
{
    m_ypr.y = pitch;
    setYPR((__bridge SCNNode*)m_native, m_ypr);
}

float NodeImpl::pitch() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return m_ypr.y;
}

void NodeImpl::roll(float roll)
{
    m_ypr.z = roll;
    setYPR((__bridge SCNNode*)m_native, m_ypr);
}

float NodeImpl::roll() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return m_ypr.z;
}

void NodeImpl::rotation(const Quaternion& rot)
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    [node setRotation:SCNVector4Make(rot.x, rot.y, rot.z, rot.w)];
}

Quaternion NodeImpl::rotation() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    auto quat = [node rotation];
    return Quaternion(quat.x,quat.y,quat.z,quat.w);
}

void NodeImpl::name(const std::string& name)
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    [node setName:[NSString stringWithUTF8String:name.c_str()]];
}

std::string NodeImpl::name() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    if (node.name == nil) {
        return "";
    }
    
    return std::string([node.name UTF8String]);
}

}
