#pragma once

#include "types.h"

class Node;

namespace mak3do {
class Node : public std::enable_shared_from_this<Node> {
public:
    Node();
        
    void add_node(NodePtr node);
    std::vector<NodePtr> nodes() const;
    
    void removeFromParent();
    NodePtr parent() const;
    
    void position(const Vec3& position);
    Vec3 position() const;
    
    void visible(bool visible);
    bool visible() const;
    
    void scale(float scale);
    float scale() const;
    
    void yaw(float yaw);
    float yaw() const;
    
    void pitch(float yaw);
    float pitch() const;
    
    void roll(float yaw);
    float roll() const;
    
    void rotation(const Quaternion& rotation);
    Quaternion rotation() const;
    
    void action(ActionPtr action);
    
    void name(const std::string& name);
    std::string name() const;
    
    
protected:
    
    NodeImplPtr m_pimpl;
    
    friend class NodeImpl;
    friend class SceneImpl;
};
}
