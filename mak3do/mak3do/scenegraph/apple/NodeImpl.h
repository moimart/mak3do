#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class NodeImpl : public Pimpl {
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
    
    void scale(const Vec3& scale);
    Vec3 scale() const;
    
    void yaw(float yaw);
    float yaw() const;
    
    void pitch(float pitch);
    float pitch() const;
    
    void roll(float roll);
    float roll() const;
    
    void rotation(const Quaternion& rotation);
    Quaternion rotation() const;
    
    void name(const std::string& name);
    std::string name() const;
    
    void geometry(GeometryPtr geometry);
    GeometryPtr geometry() const;
    
    void look_at(const Vec3& vec);
private:
    Node* m_abstract;
    
    void* m_native;
    Vec3 m_ypr { 0,0,0 };
    
    std::vector<NodePtr> m_children;
    
    NodePtr m_parent_node { nullptr };
    GeometryPtr m_geometry;
    
    friend class SceneImpl;
    friend class LightImpl;
    friend class CameraImpl;
};
}
