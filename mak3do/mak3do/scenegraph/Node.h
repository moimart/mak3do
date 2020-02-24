#pragma once

#include "types.h"

class Node;

namespace mak3do {
class Node : public Base {
public:
    Node();
    virtual ~Node() = default;
        
    void add_node(NodePtr node);
    virtual std::vector<NodePtr> nodes() const;
    
    void removeFromParent();
    NodePtr parent() const;
    
    void position(const Vec3& position);
    Vec3 position() const;
    
    void visible(bool visible);
    bool visible() const;
    
    void scale(const Vec3& scale);
    Vec3 scale() const;
    
    void yaw(float yaw);
    float yaw() const;
    
    void pitch(float yaw);
    float pitch() const;
    
    void roll(float yaw);
    float roll() const;
    
    void rotation(const Quaternion& rotation);
    Quaternion rotation() const;
    
    void action(ActionPtr action);
    
    void stop_all_actions();
    
    void name(const std::string& name);
    std::string name() const;
    
    void geometry(GeometryPtr geometry);
    GeometryPtr geometry() const;
    
    void look_at(const Vec3& lookat);
    
protected:
    
    NodeImplPtr m_pimpl;
    
    friend class NodeImpl;
    friend class SceneImpl;
    friend class GeometryImpl;
};
}
