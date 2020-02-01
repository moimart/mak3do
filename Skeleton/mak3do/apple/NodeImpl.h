#pragma once

#include "../types.h"

namespace mak3do {
class NodeImpl {
public:
    NodeImpl(Node* parent);
    ~NodeImpl();
    
    void addChild(NodePtr node);
    std::vector<NodePtr> getChildren() const;
    
    NodePtr parent() const;
    
    void removeFromParent();
    
    void position(const Vec3& position);
    Vec3 position() const;
    
    void visible(bool visible);
    bool visible() const;
    
    void scale(float scale);
    float scale() const;
    
    void yaw(float yaw);
    float yaw() const;
    
    void pitch(float pitch);
    float pitch() const;
    
    void roll(float roll);
    float roll() const;
    
    void rotation(const Quaternion& rotation);
    Quaternion rotation() const;

private:
    Node* m_abstract;
    
    void* m_native;
    Vec3 m_ypr { 0,0,0 };
    
    std::vector<NodePtr> m_children;
    
    NodePtr m_parent_node { nullptr };
    
    friend class SceneImpl;
};
}
