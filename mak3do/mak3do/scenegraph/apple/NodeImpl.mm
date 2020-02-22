#include "NodeImpl.h"
#include "GeometryImpl.h"
#include <mak3do/scenegraph/Node.h>
#include <mak3do/scenegraph/Geometry.h>
#import <SceneKit/SceneKit.h>

namespace mak3do {
NodeImpl::NodeImpl(Node* parent)
: m_abstract(parent)
{
    SCNNode* node = [SCNNode node];
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
    
    node->m_pimpl->m_parent_node =
    std::dynamic_pointer_cast<Node>(m_abstract->shared_from_this());
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
    [node setHidden:!visible];
}

bool NodeImpl::visible() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return !node.isHidden;
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

inline void set_ypr(SCNNode* node, const Vec3& ypr)
{
    Quaternion quat;
    quat.yaw_pitch_roll(ypr.x, ypr.y, ypr.z);
    
    [node setOrientation:SCNVector4Make(quat.x, quat.y, quat.z, quat.w)];
}

void NodeImpl::yaw(float yaw)
{
    m_ypr.x = yaw;
    set_ypr((__bridge SCNNode*)m_native, m_ypr);
}

float NodeImpl::yaw() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return m_ypr.x;
}

void NodeImpl::pitch(float pitch)
{
    m_ypr.y = pitch;
    set_ypr((__bridge SCNNode*)m_native, m_ypr);
}

float NodeImpl::pitch() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return m_ypr.y;
}

void NodeImpl::roll(float roll)
{
    m_ypr.z = roll;
    set_ypr((__bridge SCNNode*)m_native, m_ypr);
}

float NodeImpl::roll() const
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    
    return m_ypr.z;
}

void NodeImpl::rotation(const Quaternion& rot)
{
    SCNNode* node = (__bridge SCNNode*)m_native;
    [node setOrientation:SCNVector4Make(rot.x, rot.y, rot.z, rot.w)];
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

void NodeImpl::geometry(GeometryPtr geometry)
{
    SCNNode* __node = (__bridge SCNNode*)m_native;
    SCNGeometry* __geometry = (__bridge SCNGeometry*)geometry->m_geometry_pimpl->m_native_geometry;
    
    [__node setGeometry: __geometry];
    
    m_geometry = geometry;
}

GeometryPtr NodeImpl::geometry() const
{
    return m_geometry;
}

void NodeImpl::look_at(const Vec3& vec)
{
    SCNNode* __node = (__bridge SCNNode*)m_native;
    
    [__node lookAt:SCNVector3Make(vec.x, vec.y, vec.z)];
}

}
