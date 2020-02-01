#pragma once

#include "../types.h"

namespace mak3do {
class NodeImpl {
public:
    NodeImpl(Node* parent);
    ~NodeImpl();
    
    void addChild(NodePtr node);
    std::vector<NodePtr> getChildren() const;
    
    void setPosition(const Vec3& position);
    Vec3 getPosition() const;
private:
    void* m_native;
    
    std::vector<NodePtr> m_children;
    
    friend class SceneImpl;
};
}
